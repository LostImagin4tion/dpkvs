RECORD_VALUE_PROTO_DIR := dpkvs/core/store_value/proto
RECORD_VALUE_GEN_DIR := dpkvs/core/store_value/generated

SERVICE_PROTO_DIR := dpkvs/network/proto
SERVICE_GEN_DIR   := dpkvs/network/generated

.PHONY: generate_store_value_protos
generate_store_value_protos:
	protoc -I=$(RECORD_VALUE_PROTO_DIR) \
			--cpp_out=$(RECORD_VALUE_GEN_DIR) \
			$(RECORD_VALUE_PROTO_DIR)/store_value.proto

.PHONY: generate_network_service_protos
generate_service_protos:
	protoc -I=$(SERVICE_PROTO_DIR) \
			--cpp_out=$(SERVICE_GEN_DIR) \
    		--grpc_out=$(SERVICE_GEN_DIR) \
    		--plugin=protoc-gen-grpc=$(shell which grpc_cpp_plugin) \
    		$(SERVICE_PROTO_DIR)/dpkvs_service.proto \

	protoc -I=$(SERVICE_PROTO_DIR) \
			--cpp_out=$(SERVICE_GEN_DIR) \
			$(SERVICE_PROTO_DIR)/get_operation.proto \
			$(SERVICE_PROTO_DIR)/put_operation.proto \
			$(SERVICE_PROTO_DIR)/remove_operation.proto
