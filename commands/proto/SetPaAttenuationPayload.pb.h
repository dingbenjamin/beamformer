/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.0-dev */

#ifndef PB_SETPAATTENUATIONPAYLOAD_PB_H_INCLUDED
#define PB_SETPAATTENUATIONPAYLOAD_PB_H_INCLUDED
#include <pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _SetPaAttenuationPayload {
    uint32_t pa_id;
    float attenuation;
/* @@protoc_insertion_point(struct:SetPaAttenuationPayload) */
} SetPaAttenuationPayload;


/* Initializer values for message structs */
#define SetPaAttenuationPayload_init_default     {0u, 0}
#define SetPaAttenuationPayload_init_zero        {0, 0}

/* Field tags (for use in manual encoding/decoding) */
#define SetPaAttenuationPayload_pa_id_tag        1
#define SetPaAttenuationPayload_attenuation_tag  2

/* Struct field encoding specification for nanopb */
#define SetPaAttenuationPayload_FIELDLIST(X, a) \
X(a, STATIC, REQUIRED, FIXED32, pa_id, 1) \
X(a, STATIC, REQUIRED, FLOAT, attenuation, 2)
#define SetPaAttenuationPayload_CALLBACK NULL
#define SetPaAttenuationPayload_DEFAULT (const uint8_t*)"\x0d\x00\x00\x00\x00\x15\x00\x00\x00\x00\x00"

extern const pb_msgdesc_t SetPaAttenuationPayload_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define SetPaAttenuationPayload_fields &SetPaAttenuationPayload_msg

/* Maximum encoded size of messages (where known) */
#define SetPaAttenuationPayload_size             10

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
