/**
 * @file ops.h
 * @brief User-defined eRPC request handler and continuation types
 */
#ifndef ERPC_OPS_H
#define ERPC_OPS_H

#include "msg_buffer.h"

namespace ERpc {

// Forward declarations
class SSlot;
class ReqHandle;
class RespHandle;

/// The application-defined request handler
typedef void (*erpc_req_func_t)(ReqHandle *req_handle, void *context);

/// The continuation function
typedef void (*erpc_cont_func_t)(RespHandle *resp_handle, void *context,
                                 size_t tag);

/// The request handler types
enum class ReqFuncType : uint8_t { kFgTerminal, kFgNonterminal, kBackground };

/// The application-specified eRPC request handler
class ReqFunc {
 public:
  erpc_req_func_t req_func;
  ReqFuncType req_func_type;

  inline bool is_fg_terminal() const {
    return req_func_type == ReqFuncType::kFgTerminal;
  }

  inline bool is_background() const {
    return req_func_type == ReqFuncType::kBackground;
  }

  ReqFunc() { req_func = nullptr; }

  ReqFunc(erpc_req_func_t req_func, ReqFuncType req_func_type)
      : req_func(req_func), req_func_type(req_func_type) {
    if (req_func == nullptr) {
      throw std::runtime_error("Invalid Ops with NULL handler function");
    }
  }

  /// Check if this request handler is registered
  inline bool is_registered() const { return req_func != nullptr; }
};
}

#endif  // ERPC_OPS_H
