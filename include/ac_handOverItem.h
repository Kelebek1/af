#ifndef AC_HANDOVERITEM_H
#define AC_HANDOVERITEM_H

#include "ultra64.h"

enum aHOIRequest {
    /* 0  */ aHOI_REQUEST_NO_REQUEST,
    /* 1  */ aHOI_REQUEST_TRANSFER,
    /* 2  */ aHOI_REQUEST_TRANS_WAIT,
    /* 3  */ aHOI_REQUEST_GET,
    /* 4  */ aHOI_REQUEST_GET_PULL,
    /* 5  */ aHOI_REQUEST_EAT,
    /* 6  */ aHOI_REQUEST_CHANGE,
    /* 7  */ aHOI_REQUEST_PUTAWAY,
    /* 8  */ aHOI_REQUEST_GET_PULL_WAIT,
    /* 9  */ aHOI_REQUEST_RETURN,
    /* 10 */ aHOI_REQUEST_RETURN_WAIT,
    /* 11 */ aHOI_REQUEST_ESTIMATE,
    /* 12 */ aHOI_REQUEST_NUM,
};

#endif
