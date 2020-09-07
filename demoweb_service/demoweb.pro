TEMPLATE = subdirs
SUBDIRS = \
    demoweb/demoweb_service.pro \
    demoweb/demoweb_service_main.pro \
    _test_demoweb/_test_module/_test_create_user/_test_create_user.pro \
    _test_demoweb/_test_module/_test_message_channel/_test_message_channel.pro \
    _test_demoweb/_test_module/_test_retrieve_user/_test_retrieve_user.pro \
    _test_demoweb/_test_module/_test_user_identity/_test_user_identity.pro \
    _test_demoweb/_test_module/_test_user_profile/_test_user_profile.pro \
    _test_demoweb/_test_module/_test_file_access_validator/_test_file_access_validator.pro \
    _test_demoweb/_test_module/_test_contact_invitation/_test_contact_invitation.pro \
    _test_demoweb/_test_module/_test_retrieve_contact/_test_retrieve_contact.pro \
    _test_demoweb/_test_module/_test_search_user/_test_search_user.pro

CONFIG += ordered
