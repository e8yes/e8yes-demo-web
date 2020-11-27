TEMPLATE = subdirs
SUBDIRS = \
    demoweb/demoweb_service.pro \
    demoweb/demoweb_service_main.pro \
    _test_demoweb/_test_module/_test_baseline_user/_test_baseline_user.pro \
    _test_demoweb/_test_module/_test_message_channel_storage/_test_message_channel_storage.pro \
    _test_demoweb/_test_module/_test_message_channel/_test_message_channel.pro \
    _test_demoweb/_test_module/_test_user_storage/_test_user_storage.pro \
    _test_demoweb/_test_module/_test_user_identity/_test_user_identity.pro \
    _test_demoweb/_test_module/_test_user_profile/_test_user_profile.pro \
    _test_demoweb/_test_module/_test_file_access_validator/_test_file_access_validator.pro \
    _test_demoweb/_test_module/_test_contact_invitation/_test_contact_invitation.pro \
    _test_demoweb/_test_module/_test_contact_storage/_test_contact_storage.pro \
    _test_demoweb/_test_module/_test_search_user/_test_search_user.pro \
    _test_demoweb/_test_module/_test_chat_message_group_storage/_test_chat_message_group_storage.pro \
    _test_demoweb/_test_module/_test_chat_message_storage/_test_chat_message_storage.pro \
    _test_demoweb/_test_pbac/_test_message_channel_attributes/_test_message_channel_attributes.pro \
    _test_demoweb/_test_pbac/_test_message_channel_member_attributes/_test_message_channel_member_attributes.pro \
    _test_demoweb/_test_pbac/_test_message_channel_pbac/_test_message_channel_pbac.pro

CONFIG += ordered
