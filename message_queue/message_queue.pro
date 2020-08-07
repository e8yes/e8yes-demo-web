TEMPLATE = subdirs
SUBDIRS = \
    message_queue/message_queue_service.pro \
    message_queue/message_queue_service_main.pro \
    message_queue_test/module/message_queue_store_test \
    publisher/publisher.pro \
    subscriber/subscriber_service.pro

CONFIG += ordered
