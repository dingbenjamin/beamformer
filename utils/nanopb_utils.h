#ifndef SRC_UTIL_NANOPB_UTILS_H_
#define SRC_UTIL_NANOPB_UTILS_H_

#include <nanopb/pb_decode.h>
#include <nanopb/pb_encode.h>

#define NanopbEncode(NanopbMessageType)                              \
    NanopbUtils::Encode<NanopbMessageType, NanopbMessageType##_size, \
                        NanopbMessageType##_fields>

#define NanopbDecode(NanopbMessageType)                              \
    NanopbUtils::Decode<NanopbMessageType, NanopbMessageType##_size, \
                        NanopbMessageType##_fields>

class NanopbUtils {
   public:
    template <typename NanopbMessageType, uint16_t NanopbMessageType_size,
              const pb_field_t* NanopbMessageType_fields>
    static void Encode(uint8_t buffer[], const NanopbMessageType& message) {
        pb_ostream_t stream =
            pb_ostream_from_buffer(buffer, NanopbMessageType_size);
        // TODO(dingbenjamin): Say which nanopb message failed to encode
        if (!pb_encode(&stream, NanopbMessageType_fields, &message)) {
            throw;
        }
    }

    template <typename NanopbMessageType, uint16_t NanopbMessageType_size,
              const pb_field_t* NanopbMessageType_fields>
    static NanopbMessageType Decode(const uint8_t buffer[]) {
        pb_istream_t stream =
            pb_istream_from_buffer(buffer, NanopbMessageType_size);
        NanopbMessageType message_struct;
        // TODO(dingbenjamin): Say which nanopb message failed to decode
        if (!pb_decode(&stream, NanopbMessageType_fields, &message_struct)) {
            throw;
        }
        return message_struct;
    }
};

#endif // SRC_UTIL_NANOPB_UTILS_H_
