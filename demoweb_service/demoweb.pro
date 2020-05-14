TEMPLATE = subdirs
SUBDIRS = \
    demoweb/demowebservice.pro \
    demoweb/demowebmain.pro \
    demoweb_test/module_identity/create_user_test \
    demoweb_test/module_identity/retrieve_user_test

CONFIG += ordered
