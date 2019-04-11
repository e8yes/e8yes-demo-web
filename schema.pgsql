SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET row_security = off;

ALTER DATABASE demoweb OWNER TO postgres;

CREATE EXTENSION btree_gin;
CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;
COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';

SET search_path = public, pg_catalog;
SET default_tablespace = '';
SET default_with_oids = false;

CREATE SEQUENCE auser_id_seq
    START WITH 1024
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE auser (
    id BIGINT NOT NULL DEFAULT nextval('auser_id_seq'),
    user_name VARCHAR(256) UNIQUE NOT NULL,
    alias VARCHAR(256) NOT NULL,
    passcode VARCHAR(1024) NOT NULL,
    created_at TIMESTAMP WITHOUT TIME ZONE NOT NULL DEFAULT CURRENT_TIMESTAMP,
    avatar_path BIGINT,
    status INT NOT NULL DEFAULT 0,
    PRIMARY KEY (id)
);

CREATE INDEX idx_auser_user_name ON auser USING btree (user_name);


CREATE TABLE friend_request (
    sender_id BIGINT NOT NULL,
    receiver_id BIGINT NOT NULL,
    created_at TIMESTAMP WITHOUT TIME ZONE NOT NULL DEFAULT CURRENT_TIMESTAMP,
    status INT NOT NULL DEFAULT 0,
    PRIMARY KEY (sender_id, receiver_id),
    FOREIGN KEY (sender_id) REFERENCES auser (id) ON DELETE CASCADE,
    FOREIGN KEY (receiver_id) REFERENCES auser (id) ON DELETE CASCADE
);

CREATE INDEX idx_friend_request_created_at ON friend_request USING btree (created_at);


CREATE TABLE friend (
    sender_id BIGINT NOT NULL,
    receiver_id BIGINT NOT NULL,
    created_at TIMESTAMP WITHOUT TIME ZONE NOT NULL DEFAULT CURRENT_TIMESTAMP,
    status INT NOT NULL DEFAULT 0,
    PRIMARY KEY (sender_id, receiver_id),
    FOREIGN KEY (sender_id) REFERENCES auser (id) ON DELETE CASCADE,
    FOREIGN KEY (receiver_id) REFERENCES auser (id) ON DELETE CASCADE
);


CREATE SEQUENCE message_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE message (
    id BIGINT NOT NULL DEFAULT nextval('message_id_seq'),
    sender_id BIGINT NOT NULL,
    msg VARCHAR(2048),
    created_at TIMESTAMP WITHOUT TIME ZONE NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (id),
    FOREIGN KEY (sender_id) REFERENCES auser (id) ON DELETE CASCADE
);


CREATE SEQUENCE message_queue_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE message_quque (
    id BIGINT NOT NULL DEFAULT nextval('message_queue_id_seq'),
    message_id BIGINT NOT NULL,
    receiver_id BIGINT NOT NULL,
    status INT NOT NULL DEFAULT 0,
    PRIMARY KEY (id),
    FOREIGN KEY (message_id) REFERENCES message (id) ON DELETE CASCADE,
    FOREIGN KEY (receiver_id) REFERENCES auser (id) ON DELETE CASCADE
)