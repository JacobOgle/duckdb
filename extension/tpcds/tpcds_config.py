import os
# list all include directories
include_directories = [os.path.sep.join(x.split('/')) for x in ['extension/tpcds/include', 'extension/tpcds/dsdgen/include', 'extension/tpcds/dsdgen/include/dsdgen']]
# source files
source_files = [os.path.sep.join(x.split('/')) for x in ['extension/tpcds/tpcds-extension.cpp', 'extension/tpcds/dsdgen/dsdgen.cpp', 'extension/tpcds/dsdgen/append_info-c.cpp', 'extension/tpcds/dsdgen/dsdgen_helpers.cpp']]
source_files += [os.path.sep.join(x.split('/')) for x in ['extension/tpcds/dsdgen/dsdgen-c/skip_days.c', 'extension/tpcds/dsdgen/dsdgen-c/address.c', 'extension/tpcds/dsdgen/dsdgen-c/build_support.c', 'extension/tpcds/dsdgen/dsdgen-c/date.c', 'extension/tpcds/dsdgen/dsdgen-c/dbgen_version.c', 'extension/tpcds/dsdgen/dsdgen-c/decimal.c', 'extension/tpcds/dsdgen/dsdgen-c/dist.c', 'extension/tpcds/dsdgen/dsdgen-c/error_msg.c', 'extension/tpcds/dsdgen/dsdgen-c/genrand.c', 'extension/tpcds/dsdgen/dsdgen-c/join.c', 'extension/tpcds/dsdgen/dsdgen-c/list.c', 'extension/tpcds/dsdgen/dsdgen-c/load.c', 'extension/tpcds/dsdgen/dsdgen-c/misc.c', 'extension/tpcds/dsdgen/dsdgen-c/nulls.c', 'extension/tpcds/dsdgen/dsdgen-c/parallel.c', 'extension/tpcds/dsdgen/dsdgen-c/permute.c', 'extension/tpcds/dsdgen/dsdgen-c/pricing.c', 'extension/tpcds/dsdgen/dsdgen-c/r_params.c', 'extension/tpcds/dsdgen/dsdgen-c/release.c', 'extension/tpcds/dsdgen/dsdgen-c/scaling.c', 'extension/tpcds/dsdgen/dsdgen-c/scd.c', 'extension/tpcds/dsdgen/dsdgen-c/sparse.c', 'extension/tpcds/dsdgen/dsdgen-c/StringBuffer.c', 'extension/tpcds/dsdgen/dsdgen-c/tdef_functions.c', 'extension/tpcds/dsdgen/dsdgen-c/tdefs.c', 'extension/tpcds/dsdgen/dsdgen-c/text.c', 'extension/tpcds/dsdgen/dsdgen-c/w_call_center.c', 'extension/tpcds/dsdgen/dsdgen-c/w_catalog_page.c', 'extension/tpcds/dsdgen/dsdgen-c/w_catalog_returns.c', 'extension/tpcds/dsdgen/dsdgen-c/w_catalog_sales.c', 'extension/tpcds/dsdgen/dsdgen-c/w_customer.c', 'extension/tpcds/dsdgen/dsdgen-c/w_customer_address.c', 'extension/tpcds/dsdgen/dsdgen-c/w_customer_demographics.c', 'extension/tpcds/dsdgen/dsdgen-c/w_datetbl.c', 'extension/tpcds/dsdgen/dsdgen-c/w_household_demographics.c', 'extension/tpcds/dsdgen/dsdgen-c/w_income_band.c', 'extension/tpcds/dsdgen/dsdgen-c/w_inventory.c', 'extension/tpcds/dsdgen/dsdgen-c/w_item.c', 'extension/tpcds/dsdgen/dsdgen-c/w_promotion.c', 'extension/tpcds/dsdgen/dsdgen-c/w_reason.c', 'extension/tpcds/dsdgen/dsdgen-c/w_ship_mode.c', 'extension/tpcds/dsdgen/dsdgen-c/w_store.c', 'extension/tpcds/dsdgen/dsdgen-c/w_store_returns.c', 'extension/tpcds/dsdgen/dsdgen-c/w_store_sales.c', 'extension/tpcds/dsdgen/dsdgen-c/w_timetbl.c', 'extension/tpcds/dsdgen/dsdgen-c/w_warehouse.c', 'extension/tpcds/dsdgen/dsdgen-c/w_web_page.c', 'extension/tpcds/dsdgen/dsdgen-c/w_web_returns.c', 'extension/tpcds/dsdgen/dsdgen-c/w_web_sales.c', 'extension/tpcds/dsdgen/dsdgen-c/w_web_site.c']]