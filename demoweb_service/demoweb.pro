TEMPLATE = subdirs
SUBDIRS = \
    demoweb/demowebservice.pro \
    demoweb/demowebmain.pro \
    demoweb_test/module/create_user_test \
    demoweb_test/module/retrieve_user_test \
    demoweb_test/module/user_identity_test \
    demoweb_test/module/user_profile_test \
    demoweb_test/module/file_access_validator_test \
    demoweb_test/module/contact_invitation_test \
    demoweb_test/module/retrieve_contact_test

CONFIG += ordered
