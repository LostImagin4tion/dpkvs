STORE_VALUE_DIR := dpkvs/core/store_value
STORE_RECORD_DIR := dpkvs/core/store_record
SERVICE_DIR := dpkvs/network


.PHONY: generate_store_value_protos
generate_store_value_protos:
	protoc -I=. \
			--proto_path=$(STORE_VALUE_DIR)/proto \
			--cpp_out=$(STORE_VALUE_DIR)/generated \
			store_value.proto


.PHONY: generate_store_record_protos
generate_store_record_protos:
	protoc -I=. \
			--proto_path=$(STORE_RECORD_DIR)/proto \
            --cpp_out=$(STORE_RECORD_DIR)/generated \
			store_record.proto


.PHONY: generate_service_protos
generate_service_protos:
	protoc -I=. \
			--proto_path=$(SERVICE_DIR)/proto \
			--cpp_out=$(SERVICE_DIR)/generated \
    		--grpc_out=$(SERVICE_DIR)/generated \
    		--plugin=protoc-gen-grpc=$(shell which grpc_cpp_plugin) \
    		dpkvs_service.proto \

	protoc -I=. \
			--proto_path=$(SERVICE_DIR)/proto \
            --cpp_out=$(SERVICE_DIR)/generated \
			get_operation.proto \
			put_operation.proto \
			remove_operation.proto
