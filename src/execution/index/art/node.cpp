#include "duckdb/execution/index/art/node.hpp"
#include "duckdb/execution/index/art/art.hpp"
#include "duckdb/common/exception.hpp"

namespace duckdb {

Node::Node(NodeType type, size_t compressed_prefix_size) : prefix_length(0), count(0), type(type) {
	this->prefix = unique_ptr<uint8_t[]>(new uint8_t[compressed_prefix_size]);
}

void Node::CopyPrefix(Node *src, Node *dst) {
	dst->prefix_length = src->prefix_length;
	memcpy(dst->prefix.get(), src->prefix.get(), src->prefix_length);
}

// LCOV_EXCL_START
Node *Node::GetChild(ART &art, idx_t pos) {
	D_ASSERT(0);
	return nullptr;
}

void Node::ReplaceChildPointer(idx_t pos, Node *node) {
	D_ASSERT(0);
}

idx_t Node::GetMin() {
	D_ASSERT(0);
	return 0;
}
// LCOV_EXCL_STOP

void Node::UnswizzleChild(ART &art, SwizzleablePointer &pointer) {
	if (pointer.IsSwizzled()) {
		// This means our pointer is not yet in memory, gotta deserialize this
		// first we unset the bae
		auto block_info = pointer.GetSwizzledBlockInfo();
		AssignPointer(pointer, Deserialize(art, block_info.block_id, block_info.offset));
	}
}

Node *Node::Deserialize(ART &art, idx_t block_id, idx_t offset) {
	MetaBlockReader reader(art.db, block_id);
	reader.offset = offset;
	auto n = reader.Read<uint8_t>();
	NodeType node_type(static_cast<NodeType>(n));
	switch (node_type) {
	case NodeType::NLeaf:
		return Leaf::Deserialize(reader);
	case NodeType::N4:
		return Node4::Deserialize(reader);
	case NodeType::N16:
		return Node16::Deserialize(reader);
	case NodeType::N48:
		return Node48::Deserialize(reader);
	case NodeType::N256:
		return Node256::Deserialize(reader);
	default:
		throw InternalException("Invalid ART Node Type");
	}
}

uint32_t Node::PrefixMismatch(Node *node, Key &key, uint64_t depth) {
	uint64_t pos;
	for (pos = 0; pos < node->prefix_length; pos++) {
		if (key[depth + pos] != node->prefix[pos]) {
			return pos;
		}
	}
	return pos;
}

void Node::InsertLeaf(Node *&node, uint8_t key, Node *new_node) {
	switch (node->type) {
	case NodeType::N4:
		Node4::Insert(node, key, new_node);
		break;
	case NodeType::N16:
		Node16::Insert(node, key, new_node);
		break;
	case NodeType::N48:
		Node48::Insert(node, key, new_node);
		break;
	case NodeType::N256:
		Node256::Insert(node, key, new_node);
		break;
	default:
		throw InternalException("Unrecognized leaf type for insert");
	}
}

void Node::Erase(Node *&node, idx_t pos, ART &art) {
	switch (node->type) {
	case NodeType::N4: {
		Node4::Erase(node, pos, art);
		break;
	}
	case NodeType::N16: {
		Node16::Erase(node, pos, art);
		break;
	}
	case NodeType::N48: {
		Node48::Erase(node, pos, art);
		break;
	}
	case NodeType::N256:
		Node256::Erase(node, pos, art);
		break;
	default:
		throw InternalException("Unrecognized leaf type for erase");
	}
}

void Node::AssignPointer(SwizzleablePointer &to, Node *from) {
	if (sizeof(from) == 4) {
		to.pointer = (uint32_t)(size_t)from;
	} else {
		to.pointer = (uint64_t)from;
	}
}

SwizzleablePointer::SwizzleablePointer(idx_t block_id, idx_t offset) {
	if (block_id == DConstants::INVALID_INDEX || offset == DConstants::INVALID_INDEX) {
		pointer = 0;
		return;
	}
	idx_t pointer_size = sizeof(pointer) * 8;
	pointer = block_id;
	pointer = pointer << (pointer_size / 2);
	pointer += offset;
	// Set the left most bit to indicate this is a swizzled pointer and send it back to the mother-ship
	uint64_t mask = 1;
	mask = mask << (pointer_size - 1);
	pointer |= mask;
}

SwizzleablePointer &SwizzleablePointer::operator=(const uint64_t &ptr) {
	pointer = ptr;
	return *this;
}

bool operator!=(const SwizzleablePointer &s_ptr, const uint64_t &ptr) {
	return (s_ptr.pointer != ptr);
}

DiskPosition SwizzleablePointer::GetSwizzledBlockInfo() {
	D_ASSERT(IsSwizzled());
	idx_t pointer_size = sizeof(pointer) * 8;
	pointer = pointer & ~(1ULL << (pointer_size - 1));
	uint32_t block_id = pointer >> (pointer_size / 2);
	uint32_t offset = pointer & 0xffffffff;
	return {block_id, offset};
}
bool SwizzleablePointer::IsSwizzled() {
	idx_t pointer_size = sizeof(pointer) * 8;
	return (pointer >> (pointer_size - 1)) & 1;
}

} // namespace duckdb
