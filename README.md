# Spark2.0 Http 服务
在 nginx 上部署 `科大讯飞星火大模型V2.0` 服务，支持简单的 HTTP 请求

## 先决条件

* Linux 平台
* 了解 [ngx_http_service_module](https://github.com/xukeawsl/ngx_http_service_module) 如何工作, 在环境上编译安装好
* 在科大讯飞开放平台注册, 拥有服务接口认证信息

## 构建
1. 克隆本项目
```bash

```

2. 编译服务模块
```bash

```

## 配置
1. 配置 `config.json` 文件
```json

```

2. 配置 `nginx.conf` 文件
```nginx

```

## 启动服务
```bash

```

