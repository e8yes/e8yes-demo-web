SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET row_security = off;

CREATE EXTENSION IF NOT EXISTS btree_gin;
CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;
COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';

SET search_path = public, pg_catalog;
SET default_tablespace = '';
SET default_with_oids = false;


/* Key Generator Persistence */
CREATE TABLE IF NOT EXISTS key_persistence (
    encrypter CHARACTER VARYING(256) NOT NULL,
    key_type INT NOT NULL,
    crypto_key BYTEA NOT NULL,
    crypto_public_key BYTEA NULL
);


/* File Metadata */
CREATE TABLE IF NOT EXISTS file_metadata (
    path CHARACTER VARYING(256) NOT NULL,
    format INT NOT NULL,
    encryption_key_source INT NOT NULL,
    storage_size BIGINT NOT NULL,
    created_at TIMESTAMP WITHOUT TIME ZONE NOT NULL DEFAULT CURRENT_TIMESTAMP,
    last_modified_at TIMESTAMP WITHOUT TIME ZONE NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (path)
);


/* Email */
CREATE TABLE IF NOT EXISTS email_set (
    email CHARACTER VARYING NOT NULL,
    PRIMARY KEY (email)
);


/* User group */
CREATE TABLE IF NOT EXISTS user_group (
    group_name CHARACTER VARYING(60) NOT NULL,
    permissions INT[] NULL,
    PRIMARY KEY (group_name)
);

CREATE TABLE IF NOT EXISTS user_group_has_file (
    group_name CHARACTER VARYING(60) NOT NULL,
    file_path CHARACTER VARYING(256) NOT NULL,
    can_read BOOLEAN NOT NULL DEFAULT FALSE,
    can_write BOOLEAN NOT NULL DEFAULT FALSE,
    PRIMARY KEY (group_name, file_path),
    FOREIGN KEY (group_name) REFERENCES user_group (group_name) ON DELETE CASCADE,
    FOREIGN KEY (file_path) REFERENCES file_metadata (path) ON DELETE CASCADE
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
    id_str CHARACTER VARYING(30) NOT NULL,
    alias CHARACTER VARYING(40) NULL,
    emails CHARACTER VARYING [] NULL,
    avatar_path CHARACTER VARYING(128) NULL,
    avatar_preview_path CHARACTER VARYING(128) NULL,
    security_key_hash CHARACTER VARYING NOT NULL,
    group_names CHARACTER VARYING(60) [] NULL,
    active_level INT NOT NULL DEFAULT 0,
    created_at TIMESTAMP WITHOUT TIME ZONE NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (id),
    FOREIGN KEY (avatar_path) REFERENCES file_metadata (path) ON DELETE SET NULL,
    FOREIGN KEY (avatar_preview_path) REFERENCES file_metadata (path) ON DELETE SET NULL
);

CREATE INDEX IF NOT EXISTS idx_auser_id_str ON auser USING btree (id_str);
CREATE INDEX IF NOT EXISTS idx_auser_alias ON auser USING btree (alias);


/* Directed contact relations */
CREATE TABLE IF NOT EXISTS contact_relation (
    src_user_id BIGINT NOT NULL,
    dst_user_id BIGINT NOT NULL,
    relation INT NOT NULL DEFAULT 0,
    created_at TIMESTAMP WITHOUT TIME ZONE NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (src_user_id, dst_user_id, relation),
    FOREIGN KEY (src_user_id) REFERENCES auser (id) ON DELETE CASCADE,
    FOREIGN KEY (dst_user_id) REFERENCES auser (id) ON DELETE CASCADE
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

CREATE TABLE IF NOT EXISTS messaging_channel_has_user (
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
    media_file_path CHARACTER VARYING(128) NULL,
    media_file_preview_path CHARACTER VARYING(128) NULL,
    created_at TIMESTAMP WITHOUT TIME ZONE NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (id),
    FOREIGN KEY (channel_id) REFERENCES messaging_channel (id) ON DELETE CASCADE,
    FOREIGN KEY (sender_id) REFERENCES auser (id) ON DELETE CASCADE,
    FOREIGN KEY (media_file_path) REFERENCES file_metadata (path) ON DELETE SET NULL,
    FOREIGN KEY (media_file_preview_path) REFERENCES file_metadata (path) ON DELETE SET NULL
);
