init-linux-pkgs:
	chmod +x linux-pkgs-setup.sh
	./linux-pkgs-setup.sh
create-linux-dbs:
	chmod +x linux-db-create.sh
	./linux-db-create.sh
setup-linux-dbs:
	chmod +x linux-db-setup.sh
	./linux-db-setup.sh
init-linux: init-linux-pkgs create-linux-dbs setup-linux-dbs

build: init-linux
	mvn clean
	mvn antrun:run@compile-protoc
	mvn install -Dhttps.protocols=TLSv1.2 -Dexec.args=-ea

run: build 
	mvn exec:java -Dexec.args=-ea
test: build 

push: test
	git push
