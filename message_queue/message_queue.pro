TEMPLATE = subdirs
SUBDIRS = \
    message_queue/message_queue_service.pro \
    message_queue/message_queue_service_main.pro \
    publisher/publisher.pro \
    subscriber/subscriber_service.pro \
    _test_message_queue/_test_module/_test_message_queue_store/_test_message_queue_store.pro

CONFIG += ordered
