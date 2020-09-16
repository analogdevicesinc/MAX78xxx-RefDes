package com.maximintegrated.maxcamandroid

enum class OperationState {
    NONE,
    PROGRESS,
    SUCCESS,
    FAIL
}

enum class Operations {
    NONE,
    GET_DIRECTORY,
    GET_LS,
    GET_FILE,
    SEND_FILE,
    LOAD_IMAGE,
    CAPTURE_IMAGE,
    SEND_IMAGE,
    ENTER_DEMO,
    UPDATE_DEVICE
}
