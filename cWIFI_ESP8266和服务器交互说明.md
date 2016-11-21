# 关于WIFI模块和服务器交互AT指令集说明

## 测试
AT

模式配置AP+STA=3 STA=1
AT+CWMODE=3

重启
AT+RST

重启后：

单路连接
AT+CIPMUX=0

不透明传输
AT+CIPMODE=0

列出周围的WIFI帐号
AT+CWLAP

返回数据：
+CWLAP:(0,"tongji.portal",-76,"3c:e5:a6:9f:9e:b2",1,-16)
+CWLAP:(4,"623network5_8GHz",-44,"ec:6c:9f:20:7d:f2",5,-16)
+CWLAP:(3,"YD-Xiaoxin",-44,"da:ff:28:68:47:9d",5,1)

OK
(此处是一个空行)

连接WIFI
AT+CWJAP="623network5_8GHz","6958949599"
AT+CWJAP="623network","6958949599"
AT+CWJAP="Dong.Yang","12341234"
AT+CWJAP="YD-Xiaoxin","12341234"

返回数据：
WIFI CONNECTED
WIFI GOT IP

OK
(此处是一个空行)

查询wifi连接情况：
AT+CWJAP?

返回数据：
+CWJAP:"YD-Xiaoxin","da:ff:28:68:47:9d",5,-43

OK
(此处是一个空行)

连接服务器
AT+CIPSTART="TCP","183.230.40.33",80
AT+CIPSTART="TCP","alexyan.xyz",4000
AT+CIPSTART="TCP","104.128.89.30",4000

返回数据：
AT+CIPSTART="TCP","alexyan.xyz",4000
CONNECT

OK
(此处是一个空行)

发送数据
AT+CIPSEND=67

返回数据：
AT+CIPSEND=67

OK
>   （此处还有一个空格）

发送数据：必须加上两个空行
GET /api/hardware/report/HW001 HTTP/1.1
Host: alexyan.xyz:4000


返回数据：
Recv 67 bytes

SEND OK

+IPD,369:HTTP/1.1 200 OK
X-Powered-By: Express
Content-Type: application/json; charset=utf-8
Content-Length: 1
ETag: W/"1-z80ghJXVZe9m59/5+Ydk2g"
set-cookie: connect.sid=s%3ARpF-8dedgNqCOO6g47nWXPCRt7Y6gyGG.q7cxm50FWoNHbq1oYH7zwcpe7yDQzDPq2GyoQba96ac; Path=/; Expires=Mon, 21 Nov 2016 05:31:59 GMT; HttpOnly
Date: Mon, 21 Nov 2016 05:30:59 GMT
Connection: keep-alive

0（只有字符“0”，无其他字符）

发送数据
AT+CIPSEND=86

发送数据：必须加上两个空行
GET /api/hardware/download/HW001 HTTP/1.1
Host: alexyan.xyz:4000
Range: byte=0-5


发送数据
AT+CIPSEND=67

发送数据：必须加上两个空行
GET /api/hardware/finish/HW001 HTTP/1.1
Host: alexyan.xyz:4000


AT+CIPSTART="TCP","alexyan.xyz",4000

发送数据
AT+CIPSEND=196

发送数据：必须加上两个空行
POST /api/hardware/image HTTP/1.1
Host: alexyan.xyz:4000
Content-Type: application/json
Content-Length: 79

{"id": "HW004",
"image":{
"size": 4724,
"path":"public/HelloWorld.bin"
}
}


返回数据：
Recv 196 bytes

SEND OK

+IPD,291:HTTP/1.1 200 OK
X-Powered-By: Express
set-cookie: connect.sid=s%3AEp5uZuGK0-TfVG2yA2EX4XAkPgJtM-Bt.zHLGQgt9%2FqqBaqcnmX9DG5BUl%2F9NzswBj%2BLeO5FeKaI; Path=/; Expires=Mon, 21 Nov 2016 12:50:48 GMT; HttpOnly
Date: Mon, 21 Nov 2016 12:49:48 GMT
Connection: keep-alive
Content-Length: 0



AT+CIPSTART="TCP","alexyan.xyz",4000

发送数据
AT+CIPSEND=61

发送数据：必须加上两个空行
GET /api/hardware/status HTTP/1.1
Host: alexyan.xyz:4000


返回数据：
Recv 61 bytes

SEND OK

+IPD,371:HTTP/1.1 200 OK
X-Powered-By: Express
Content-Type: application/json; charset=utf-8
Content-Length: 114
ETag: W/"72-G8zBngYQDRe7xJsiWIcfxw"
set-cookie: connect.sid=s%3AikMhRs-Ix-HyO7ISRZBF6bTbAkG2BTWU.NcxAj5HW1EJX3o2dhKBgfMnkEXeBv2ISU6tOAYFL5FY; Path=/; Expires=Mon, 21 Nov 2016 12:51:19 GMT; HttpOnly
Date: Mon, 21 Nov 2016 12:50:19 GMT
Connection: keep-alive


+IPD,114:{"hdReport":{"HW001":"2016-11-21T12:07:36.946Z"},"hdImage":{"HW004":{"size":4724,"path":"public/HelloWorld.bin"}}}


关闭单链接
AT+CIPCLOSE
