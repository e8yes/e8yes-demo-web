SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET row_security = off;

ALTER DATABASE demoweb OWNER TO postgres;

CREATE EXTENSION IF NOT EXISTS btree_gin;
CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;
COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';

SET search_path = public, pg_catalog;
SET default_tablespace = '';
SET default_with_oids = false;


/* File */
CREATE SEQUENCE IF NOT EXISTS file_id_seq
    START WITH 100
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE IF NOT EXISTS file (
    id BIGINT NOT NULL DEFAULT nextval('file_id_seq'),
    volumn INT NOT NULL,
    format INT NOT NULL,
    encryption_key_source INT NOT NULL,
    created_at TIMESTAMP WITHOUT TIME ZONE NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (id)
);


/* Email */
CREATE TABLE IF NOT EXISTS email_set (
    email CHARACTER VARYING NOT NULL,
    PRIMARY KEY (email)
);


/* User group */
CREATE SEQUENCE IF NOT EXISTS auser_group_id_seq
    START WITH 32
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE IF NOT EXISTS auser_group (
    group_name VARCHAR(60) NOT NULL,
    permissions INT[] NULL,
    PRIMARY KEY (group_name)
);


/* User */
CREATE SEQUENCE IF NOT EXISTS auser_id_seq
    START WITH 1024
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE IF NOT EXISTS auser (
    id BIGINT NOT NULL DEFAULT nextval('auser_id_seq'),
    emails CHARACTER VARYING [] NULL,
    alias CHARACTER VARYING(40) NULL,
    avatar_file_id BIGINT NULL,
    security_key_hash CHARACTER VARYING NOT NULL,
    group_names CHARACTER VARYING(60) [] NULL,
    active_level INT NOT NULL DEFAULT 0,
    created_at TIMESTAMP WITHOUT TIME ZONE NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (id),
    FOREIGN KEY (avatar_file_id) REFERENCES file (id) ON DELETE SET NULL
);

CREATE INDEX IF NOT EXISTS idx_auser_alias ON auser USING btree (alias);


/* Friendship */
CREATE TABLE IF NOT EXISTS friend (
    /* We requires that user0_id < user1_id. */
    user0_id BIGINT NOT NULL,
    user1_id BIGINT NOT NULL,
    created_at TIMESTAMP WITHOUT TIME ZONE NOT NULL DEFAULT CURRENT_TIMESTAMP,
    status INT NOT NULL DEFAULT 0,
    PRIMARY KEY (user0_id, user1_id),
    FOREIGN KEY (user0_id) REFERENCES auser (id) ON DELETE CASCADE,
    FOREIGN KEY (user1_id) REFERENCES auser (id) ON DELETE CASCADE
);


/* Messaging channel */
CREATE SEQUENCE IF NOT EXISTS messaging_channel_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE IF NOT EXISTS messaging_channel (
    id BIGINT NOT NULL DEFAULT nextval('messaging_channel_id_seq'),
    channel_name CHARACTER VARYING(40),
    created_at TIMESTAMP WITHOUT TIME ZONE NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (id)
);

CREATE TABLE IF NOT EXISTS messaging_channel_has_users (
    channel_id BIGINT NOT NULL,
    user_id BIGINT NOT NULL,
    ownership INT NOT NULL,
    PRIMARY KEY (channel_id, user_id),
    FOREIGN KEY (channel_id) REFERENCES messaging_channel (id) ON DELETE CASCADE,
    FOREIGN KEY (user_id) REFERENCES auser (id) ON DELETE CASCADE
);


/* Message */
CREATE SEQUENCE IF NOT EXISTS message_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE IF NOT EXISTS message (
    id BIGINT NOT NULL DEFAULT nextval('message_id_seq'),
    channel_id BIGINT NOT NULL,
    sender_id BIGINT NOT NULL,
    encrypted_content CHARACTER VARYING,
    media_file_id BIGINT NULL,
    created_at TIMESTAMP WITHOUT TIME ZONE NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (id),
    FOREIGN KEY (channel_id) REFERENCES messaging_channel (id) ON DELETE CASCADE,
    FOREIGN KEY (sender_id) REFERENCES auser (id) ON DELETE CASCADE,
    FOREIGN KEY (media_file_id) REFERENCES file (id) ON DELETE SET NULL
);
