#ifndef DSDEF_H
#define DSDEF_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#define DS_CMD "cmd"
#define DS_QUESTION "question"
#define DS_USERNAME "username"
#define DS_PASSWORD "password"
#define DS_APP "app"
#define DS_HOSTNAME "hostname"
#define DS_DATABASE "database"
#define DS_CAPTURE "capture"
#define DS_START "start"
#define DS_STOP "stop"
#define DS_STOPACK "stopack"
#define DS_SCALE "scale"
#define DS_INTERVAL "interval"
#define DS_SHM_KEY "dssharememorykey123"
#define DS_SETUP "setup"
#define DS_WIDTH "width"
#define DS_HEIGHT "height"
#define DS_RESULT "result"
#define DS_VERIFY "verify"
#define DS_OK "ok"
#define DS_ERR "err"
#define DS_REASON "reason"
#define DS_LOGIN "login"
#define DS_QQ "qq"
#define DS_MOBILE "mobile"
#define DS_REGISTER "reg"
#define DS_SESSION "session"
#define DS_ADURL "adurl"
#define DS_CHECKSESSION "checksession"
#define DS_DATE "date"
#define DS_VERSION "version"

#define DS_TMP_AUDIO "tmp_audio"
#define DS_TMP_VIDEO "tmp_video"
#define DS_TMP_META "tmp_meta"

#define DS_SHARE_ADDR "广播地址"

#define DS_YES "是"
#define DS_NO "否"

#define DS_SHARE_MODE "共享模式"
#define DS_TEXT_MODE "文本模式"
#define DS_IMAGE_MODE "图像模式"
#define DS_HD_MODE "高清模式"

#define DS_SHARE_FILE_NAME "/ds_share"
#define DS_MAP_LENGTH 33554432

#define DS_PORT_CAP 23456
#define DS_PORT_DS 23457
#define DS_PORT_SEND 23458
#define DS_PORT_VIEW 24001
#define DS_PORT_HTTPS 24002
#define DS_PORT_HTTP 24003

#endif // DSDEF_H
