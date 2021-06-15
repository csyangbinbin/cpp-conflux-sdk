add_executable(account account.cpp)
target_link_libraries(account cpp_conflux_sdk)

add_executable(send_transaction send_transaction.cpp)
target_link_libraries(send_transaction  cpp_conflux_sdk)

add_executable(abi abi.cpp)
target_link_libraries(abi  cpp_conflux_sdk)

add_executable(deploy_contract deploy_contract.cpp)
target_link_libraries(deploy_contract  cpp_conflux_sdk)

add_executable(transaction_receipt transaction_receipt.cpp)
target_link_libraries(transaction_receipt  cpp_conflux_sdk)

add_executable(bloom_filter bloom_filter.cpp)
target_link_libraries(bloom_filter  cpp_conflux_sdk)

add_executable(log_filter log_filter.cpp)
target_link_libraries(log_filter  cpp_conflux_sdk)

add_executable(currency currency.cpp)
target_link_libraries(currency  cpp_conflux_sdk)


add_executable(rpc rpc.cpp)
target_link_libraries(rpc  cpp_conflux_sdk)

add_executable(address address.cpp)
target_link_libraries(address  cpp_conflux_sdk)

add_subdirectory(qt)

