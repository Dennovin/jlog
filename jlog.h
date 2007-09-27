#ifndef _JLOG_H
#define _JLOG_H

#include "jlog_config.h"

#ifndef JLOG_API
# ifdef _WIN32
#  ifdef JLOG_EXPORTS
#   define JLOG_API(x) __declspec(dllexport) x
#  else
#   define JLOG_API(x) __declspec(dllimport) x
#  endif
# else
#  define JLOG_API(x)  x
# endif
#endif

struct _jlog_ctx;
struct _jlog_message_header;
struct _jlog_id;

typedef struct _jlog_ctx jlog_ctx;

typedef struct _jlog_message_header {
  u_int32_t reserved;
  u_int32_t tv_sec;
  u_int32_t tv_usec;
  u_int32_t mlen;
} jlog_message_header;

typedef struct _jlog_id {
  u_int32_t log;
  u_int32_t marker;
} jlog_id;

#define JLOG_ID_ADVANCE(id) (id)->marker++

typedef struct _jlog_message {
  jlog_message_header *header;
  u_int32_t mess_len;
  void *mess;
  jlog_message_header aligned_header;
} jlog_message;

typedef enum {
  JLOG_BEGIN,
  JLOG_END
} jlog_position;

typedef enum {
  JLOG_UNSAFE,
  JLOG_ALMOST_SAFE,
  JLOG_SAFE
} jlog_safety;

typedef enum {
  JLOG_ERR_SUCCESS = 0,
  JLOG_ERR_ILLEGAL_INIT,
  JLOG_ERR_ILLEGAL_OPEN,
  JLOG_ERR_OPEN,
  JLOG_ERR_NOTDIR,
  JLOG_ERR_CREATE_PATHLEN,
  JLOG_ERR_CREATE_EXISTS,
  JLOG_ERR_CREATE_MKDIR,
  JLOG_ERR_CREATE_META,
  JLOG_ERR_LOCK,
  JLOG_ERR_IDX_OPEN,
  JLOG_ERR_IDX_SEEK,
  JLOG_ERR_IDX_CORRUPT,
  JLOG_ERR_IDX_WRITE,
  JLOG_ERR_IDX_READ,
  JLOG_ERR_FILE_OPEN,
  JLOG_ERR_FILE_SEEK,
  JLOG_ERR_FILE_CORRUPT,
  JLOG_ERR_FILE_READ,
  JLOG_ERR_FILE_WRITE,
  JLOG_ERR_META_OPEN,
  JLOG_ERR_ILLEGAL_WRITE,
  JLOG_ERR_ILLEGAL_CHECKPOINT,
  JLOG_ERR_INVALID_SUBSCRIBER,
  JLOG_ERR_ILLEGAL_LOGID,
  JLOG_ERR_SUBSCRIBER_EXISTS,
  JLOG_ERR_CHECKPOINT,
  JLOG_ERR_NOT_SUPPORTED,
} jlog_err;

typedef void (*jlog_error_func) (void *ctx, const char *msg, ...);

JLOG_API(jlog_ctx *) jlog_new(const char *path);
JLOG_API(void)      jlog_set_error_func(jlog_ctx *ctx, jlog_error_func Func, void *ptr); 
JLOG_API(size_t)    jlog_raw_size(jlog_ctx *ctx);
JLOG_API(int)       jlog_ctx_init(jlog_ctx *ctx);
JLOG_API(int)       jlog_get_checkpoint(jlog_ctx *ctx, const char *s, jlog_id *id);
JLOG_API(int)       jlog_ctx_list_subscribers_dispose(jlog_ctx *ctx, char **subs);
JLOG_API(int)       jlog_ctx_list_subscribers(jlog_ctx *ctx, char ***subs);

JLOG_API(int)       jlog_ctx_err(jlog_ctx *ctx);
JLOG_API(const char *) jlog_ctx_err_string(jlog_ctx *ctx);
JLOG_API(int)       jlog_ctx_errno(jlog_ctx *ctx);
JLOG_API(int)       jlog_ctx_open_writer(jlog_ctx *ctx);
JLOG_API(int)       jlog_ctx_open_reader(jlog_ctx *ctx, const char *subscriber);
JLOG_API(int)       jlog_ctx_close(jlog_ctx *ctx);

JLOG_API(int)       jlog_ctx_alter_mode(jlog_ctx *ctx, int mode);
JLOG_API(int)       jlog_ctx_alter_journal_size(jlog_ctx *ctx, size_t size);
JLOG_API(int)       jlog_ctx_alter_safety(jlog_ctx *ctx, jlog_safety safety);
JLOG_API(int)       jlog_ctx_add_subscriber(jlog_ctx *ctx, const char *subscriber,
                                            jlog_position whence);
JLOG_API(int)       jlog_ctx_remove_subscriber(jlog_ctx *ctx, const char *subscriber);

JLOG_API(int)       jlog_ctx_write(jlog_ctx *ctx, const void *message, size_t mess_len);
JLOG_API(int)       jlog_ctx_write_message(jlog_ctx *ctx, jlog_message *msg, struct timeval *when);
JLOG_API(int)       jlog_ctx_read_interval(jlog_ctx *ctx,
                                           jlog_id *first_mess, jlog_id *last_mess);
JLOG_API(int)       jlog_ctx_read_message(jlog_ctx *ctx, const jlog_id *, jlog_message *);
JLOG_API(int)       jlog_ctx_read_checkpoint(jlog_ctx *ctx, const jlog_id *checkpoint);
JLOG_API(int)       jlog_snprint_logid(char *buff, int n, const jlog_id *checkpoint);

JLOG_API(int)       __jlog_pending_readers(jlog_ctx *ctx, u_int32_t log);
JLOG_API(int)       jlog_ctx_last_log_id(jlog_ctx *ctx, jlog_id *id);

#endif
