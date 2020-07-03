TEMPLATE = subdirs
SUBDIRS = \
    demoweb/demowebservice.pro \
    demoweb/demowebmain.pro \
    demoweb_test/module_identity/create_user_test \
    demoweb_test/module_identity/retrieve_user_test \
    demoweb_test/module_identity/user_identity_test \
    demoweb_test/module_identity/user_profile_test \
    demoweb_test/module_rbac/file_access_validator_test \
    demoweb_test/module_socialnetwork/contact_invitation_test

CONFIG += ordered
