PROTO_DIR := dpkvs/network/proto
GEN_DIR   := dpkvs/network/generated

.PHONY: generate_network_service_protos
generate_network_service_protos:
	protoc -I=$(PROTO_DIR) \
			--cpp_out=$(GEN_DIR) \
    		--grpc_out=$(GEN_DIR) \
    		--plugin=protoc-gen-grpc=$(shell which grpc_cpp_plugin) \
    		$(PROTO_DIR)/dpkvs_service.proto \

	protoc -I=$(PROTO_DIR) \
			--cpp_out=$(GEN_DIR) \
			$(PROTO_DIR)/get_operation.proto \
			$(PROTO_DIR)/put_operation.proto \
			$(PROTO_DIR)/remove_operation.proto
