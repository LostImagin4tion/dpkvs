STORE_VALUE_DIR := dpkvs/core/store_value
STORE_RECORD_DIR := dpkvs/core/store_record
SERVICE_DIR := dpkvs/network

.PHONY: env
env:
	cp configs/build/local_example.env configs/build/local.env
	cp configs/build/deploy_example.env configs/build/deploy.env


.PHONY: run
run:
	cmake --build build --target dpkvs_server -j 10
	env $$(cat configs/build/local.env | xargs) ./build/dpkvs_server


# ====== DEPLOY ======

.PHONY: deploy
docker-deploy:
	docker compose -f ./deployment/docker/docker-compose.yml up


.PHONY: k8s-deploy
k8s-deploy:
	kubectl apply -k deployment/k8s/


.PHONY: k8s-undeploy
k8s-undeploy:
	kubectl delete -k deployment/k8s/


.PHONY: k8s-logs
k8s-logs:
	kubectl logs -n dpkvs -l app=dpkvs-server --tail=100 -f


.PHONY: k8s-status
k8s-status:
	kubectl get all,pv,pvc -n dpkvs


# ====== PROTOS ======

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
			--proto_path=$(STORE_VALUE_DIR)/proto \
            --cpp_out=$(STORE_RECORD_DIR)/generated \
			store_record.proto


.PHONY: generate_service_protos
generate_service_protos:
	protoc -I=. \
			--proto_path=$(SERVICE_DIR)/proto \
			--cpp_out=$(SERVICE_DIR)/generated \
    		--grpc_out=$(SERVICE_DIR)/generated \
    		--plugin=protoc-gen-grpc=$(shell which grpc_cpp_plugin) \
    		dpkvs_service.proto

	protoc -I=. \
			--proto_path=$(SERVICE_DIR)/proto \
            --cpp_out=$(SERVICE_DIR)/generated \
			get_operation.proto \
			put_operation.proto \
			remove_operation.proto
