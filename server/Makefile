init-linux-pkgs:
	sudo apt install -y maven
	sudo apt install -y postgresql
create-linux-dbs:
	chmod +x linux-db-create.sh
	./linux-db-create.sh
setup-linux-dbs:
	chmod +x linux-db-setup.sh
	./linux-db-setup.sh
init-linux: init-linux-pkgs create-linux-dbs setup-linux-dbs

install-maven-pkgs: init-linux
	mvn install
run: install-maven-pkgs
	mvn package
	java -jar target/demoweb-server-0.1.1-jar-with-dependencies.jar
test: install-maven-pkgs
