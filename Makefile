RECORD_PROTO_DIR := dpkvs/core/store_record/proto
RECORD_GEN_DIR := dpkvs/core/store_record/generated

SERVICE_PROTO_DIR := dpkvs/network/proto
SERVICE_GEN_DIR   := dpkvs/network/generated

.PHONY: generate_record_protos
generate_record_protos:
	protoc -I=$(RECORD_PROTO_DIR) \
			--cpp_out=$(RECORD_GEN_DIR) \
			$(RECORD_PROTO_DIR)/store_record.proto

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
