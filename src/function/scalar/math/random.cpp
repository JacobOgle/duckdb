#include "duckdb/function/scalar/math_functions.hpp"
#include "duckdb/common/vector_operations/vector_operations.hpp"
#include "duckdb/execution/expression_executor.hpp"
#include "duckdb/main/client_context.hpp"
#include "duckdb/planner/expression/bound_function_expression.hpp"
#include "duckdb/common/random_engine.hpp"
#include "duckdb/function/scalar/uuid_functions.hpp"
#include "duckdb/common/types/uuid.hpp"

namespace duckdb {

struct RandomBindData : public FunctionData {
	ClientContext &context;

	RandomBindData(ClientContext &context) : context(context) {
	}

	unique_ptr<FunctionData> Copy() override {
		return make_unique<RandomBindData>(context);
	}
};

struct RandomLocalState : public FunctionData {
	explicit RandomLocalState(uint32_t seed) : random_engine(seed) {
	}

	RandomEngine random_engine;
};

static void RandomFunction(DataChunk &args, ExpressionState &state, Vector &result) {
	D_ASSERT(args.ColumnCount() == 0);
	auto &lstate = (RandomLocalState &)*ExecuteFunctionState::GetFunctionState(state);

	result.SetVectorType(VectorType::FLAT_VECTOR);
	auto result_data = FlatVector::GetData<double>(result);
	for (idx_t i = 0; i < args.size(); i++) {
		result_data[i] = lstate.random_engine.NextRandom();
	}
}

unique_ptr<FunctionData> RandomBind(ClientContext &context, ScalarFunction &bound_function,
                                    vector<unique_ptr<Expression>> &arguments) {
	return make_unique<RandomBindData>(context);
}

static unique_ptr<FunctionData> RandomInitLocalState(const BoundFunctionExpression &expr, FunctionData *bind_data) {
	auto &info = (RandomBindData &)*bind_data;
	auto &random_engine = RandomEngine::Get(info.context);
	return make_unique<RandomLocalState>(random_engine.NextRandomInteger());
}

void RandomFun::RegisterFunction(BuiltinFunctions &set) {
	set.AddFunction(ScalarFunction("random", {}, LogicalType::DOUBLE, RandomFunction, true, RandomBind, nullptr,
	                               nullptr, RandomInitLocalState));
}

static void GenerateUUIDFunction(DataChunk &args, ExpressionState &state, Vector &result) {
	D_ASSERT(args.ColumnCount() == 0);
	auto &lstate = (RandomLocalState &)*ExecuteFunctionState::GetFunctionState(state);

	result.SetVectorType(VectorType::FLAT_VECTOR);
	auto result_data = FlatVector::GetData<hugeint_t>(result);

	for (idx_t i = 0; i < args.size(); i++) {
		uint8_t bytes[16];
		for (int i = 0; i < 16; i += 4) {
			*reinterpret_cast<uint32_t *>(bytes + i) = lstate.random_engine.NextRandomInteger();
		}
		// variant must be 10xxxxxx
		bytes[8] &= 0xBF;
		bytes[8] |= 0x80;
		// version must be 0100xxxx
		bytes[6] &= 0x4F;
		bytes[6] |= 0x40;

		result_data[i].upper = 0;
		result_data[i].upper |= ((int64_t)bytes[0] << 56);
		result_data[i].upper |= ((int64_t)bytes[1] << 48);
		result_data[i].upper |= ((int64_t)bytes[3] << 40);
		result_data[i].upper |= ((int64_t)bytes[4] << 32);
		result_data[i].upper |= ((int64_t)bytes[5] << 24);
		result_data[i].upper |= ((int64_t)bytes[6] << 16);
		result_data[i].upper |= ((int64_t)bytes[7] << 8);
		result_data[i].upper |= bytes[8];
		result_data[i].lower = 0;
		result_data[i].lower |= ((uint64_t)bytes[8] << 56);
		result_data[i].lower |= ((uint64_t)bytes[9] << 48);
		result_data[i].lower |= ((uint64_t)bytes[10] << 40);
		result_data[i].lower |= ((uint64_t)bytes[11] << 32);
		result_data[i].lower |= ((uint64_t)bytes[12] << 24);
		result_data[i].lower |= ((uint64_t)bytes[13] << 16);
		result_data[i].lower |= ((uint64_t)bytes[14] << 8);
		result_data[i].lower |= bytes[15];
	}
}

void UUIDFun::RegisterFunction(BuiltinFunctions &set) {
	ScalarFunction uuid_function({}, LogicalType::UUID, GenerateUUIDFunction, false, true, RandomBind, nullptr, nullptr,
	                             RandomInitLocalState);
	// generate a random uuid
	set.AddFunction({"uuid", "gen_random_uuid"}, uuid_function);
}

} // namespace duckdb
