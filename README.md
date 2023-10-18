# Spark2.0 Http 服务

[![License](https://img.shields.io/npm/l/mithril.svg)](https://github.com/xukeawsl/Spark_Http_Service/blob/master/LICENSE)

在 nginx 上部署 `科大讯飞星火大模型V2.0` 服务，支持简单的 HTTP 请求

## 接口请求
1. 请求参数
```json
{
    "appID": "从开放平台控制台获取的appID(非必填)",
    "apiKey": "从开放平台控制台获取的apiKey(非必填)",
    "apiSecret": "从开放平台控制台获取的apiSecret(非必填)",
    "content": "你的问题(必填)"
}
```

2. 响应参数
* 错误码可能的内容参考[文档](https://www.xfyun.cn/doc/spark/LinuxSDK.html#_8-%E9%94%99%E8%AF%AF%E7%A0%81)
```json
{
    "code": 0,
    "message": "错误信息",
    "content": "星火认知大模型的回复内容"
}
```


## 先决条件

* Linux 平台
* 了解 [ngx_http_service_module](https://github.com/xukeawsl/ngx_http_service_module) 如何工作, 在环境上编译安装好
* 在科大讯飞开放平台注册, 拥有服务接口认证信息

## 构建
1. 下载
```bash
git clone https://github.com/xukeawsl/Spark_Http_Service.git
```

2. 编译服务模块
```bash
cd Spark_Http_Service
mkdir build && cd build
cmake ..
make
```

## 配置
1. 配置 `config.json` 文件
* HTTP 请求如果传了服务接口认证信息就不需要配置文件
* 如果希望没传认证信息的请求使用一个统一的就可以在文件中配置
```json
{
    "appID": "your appID",
    "apiKey": "your apiKey",
    "apiSecret": "your apiSecret"
}
```

2. 配置 `nginx.conf` 文件
* 为了使 nginx 支持本服务, 需要在配置文件的 `http` 块中进行配置
* 假设 `build` 目录的绝对路径是 `root/Spark_Http_Service/build`
* `module_path` 块中配置动态库路径
* `module_dependency` 配置模块依赖, 本模块依赖科大讯飞三方 sdk
* `service` 配置模块支持的服务, 这里是 `srv_spark`
* 最后在指定的 `location` 块中通过 `service_mode on` 来开启服务即可
```nginx
http {
    module_path {
        /root/Spark_Http_Service/build  libsrv_spark.so;
        /root/Spark_Http_Service/sdk/Spark2.0_Linux_SDK_v1.0/lib   libSparkChain.so;
    }

    module_dependency {
        libsrv_spark.so  libSparkChain.so;
    }

    service {
        libsrv_spark.so  srv_spark;
    }

    server {
        # 省略一部分配置

        location /spark_service {
            service_mode on;
        }
    }
}
```

## 启动服务
* 假如 nginx 的安装目录为 `/usr/local/nginx/sbin/`
```bash
# 测试配置是否正确
/usr/local/nginx/sbin/nginx -t

# 启动 nginx
/usr/local/nginx/sbin/nginx
```

## 平滑重启服务
```bash
/usr/local/nginx/sbin/nginx -s reload
```

## 停止服务
```bash
/usr/local/nginx/sbin/nginx -s quit
```

