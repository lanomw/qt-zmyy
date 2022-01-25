# 知苗易约爬取api
1. 数据获取
> https://cloud.cn2030.com/sc/wx/HandlerSubscribe.ashx
2. 订单提交
> https://cloud.cn2030.com/sc/api/User/OrderPost
> cookie是在响应头中获取`Set-Cookie`

### 请求带上参数

| 字段   | 类型     | 解释                               |
|:-----|:-------|:---------------------------------|
| ASP.NET_SessionId  | string | 登录凭证                             |
| HttpOnly | string |                                  |
| SameSite | string | `Lax,ASP.NET_SessionId=`后面接上授权 |
| path | string | /                                |


### 点击预约时的接口调用顺序
1. 获取预约时间的数据，需要解密: /sc/wx/HandlerSubscribe.ashx?act=GetCustSubscribeDateDetail
2. 获取验证码:/sc/wx/HandlerSubscribe.ashx?act=GetCaptcha
3. 提交预约信息，需要加密: /sc/api/User/OrderPost
4. 查询预约。可能返回408需要重新授权：/sc/wx/HandlerSubscribe.ashx?act=GetOrderStatus
5. 重新授权：/sc/wx/HandlerSubscribe.ashx
4. 查询预约。可能返回408需要重新授权：/sc/wx/HandlerSubscribe.ashx?act=GetOrderStatus

### 获取授权
> POST /sc/wx/HandlerSubscribe.ashx?act=auth&code=
> Cookie: ASP.NET_SessionId=1oxdnbak5u4y4tvpuwo15u3g; path=/; HttpOnly; SameSite=Lax,ASP.NET_SessionId=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpYXQiOjE2NDA5NDA3MjYuOTYzNjIwNCwiZXhwIjoxNjQwOTQ0MzI2Ljk2MzYyMDQsInN1YiI6IllOVy5WSVAiLCJqdGkiOiIyMDIxMTIzMTA0NTIwNiIsInZhbCI6InFSOG5BUUlBQUFBUU1tVmpNemszWXpGa05qTTFZVGt3TlJ4dmNYSTFielZJZVdWMk9VZFZhVXN0YW1jNFNVZFdWMmhKYlVabkFCeHZcclxuVlRJMldIUTFPR05UWVVnNFJrNUtjSHA0ZFdvd1dETkhZakZKRGpFM01TNHlNVEl1TWpReUxqRTFBQkJ4VWpodVFXWmxZa0ZCUkVwcFxyXG5SRkZDQVFBQUFBQT0ifQ.FwRz4T-onfN06DkDvpscItTJJCeoG6rDufdPcNxHkIE; path=/

- _请求参数_

| 字段   | 类型     | 解释                    |
|:-----|:-------|:----------------------|
| act  | string | 请求行为。固定`auth`         |
| code | string | 微信登录凭证。调用`wx.login`获取 |


- _响应参数_

| 字段     | 类型     | 解释            |
|:-------|:-------|:--------------|
| sessionId    | string | 登录凭证，接口请求需要带上 |
| msg    | string | 授权成功          |
| status | number | 200           |


### 用户信息
> GET /wx/HandlerSubscribe.ashx?act=User

- _请求参数_

| 字段   | 类型     | 解释                    |
|:-----|:-------|:----------------------|
| act  | string | 请求行为。固定`User`         |


### 修改用户信息
> GET /sc/wx/HandlerSubscribe.ashx?act=Reg&birthday=&tel=&cname=&sex=&idcard=&doctype=&age=

- _请求参数_

| 字段                    | 类型     | 解释                             |
|:----------------------|:-------|:-------------------------------|
| act                   | string | 请求行为。固定`Reg`                   |
| cname                 | string | 姓名                             |
| sex                   | number | 性别。`1: 男; 2: 女`                |
| doctype               | number | 证件类型。`1:身份证,2:护照;3:港澳证件;4:台胞证` |
| idcard | string | 证件号码                           |
| birthday | string | 出生日期                           |
| tel     | string | 电话                             |
| age        | number | 年龄                             |



### 获取一级分类列表
> GET /sc/wx/HandlerSubscribe.ashx?act=GetCat1

- _请求参数_

| 字段    | 类型     | 解释               |
|:------|:-------|:-----------------|
| act    | string | 请求行为。固定`GetCat1` |

- _响应参数_

| 字段    | 类型     | 解释   |
|:------|:-------|:-----|
| id    | number | id   |
| cname | string | 分类名称 |


### 获取分类列表
> GET /sc/wx/HandlerSubscribe.ashx?act=GetCat2&id=

- _请求参数_

| 字段    | 类型     | 解释               |
|:------|:-------|:-----------------|
| act    | string | 请求行为。固定`GetCat2` |
| id    | string | 一级分类id           |

- _响应参数_

| 字段    | 类型     | 解释   |
|:------|:-------|:-----|
| id    | number | id   |
| alias | string | 分类名称 |


### 获取医院列表
> GET /sc/wx/HandlerSubscribe.ashx?act=CustomerList&city=&lat=lng=&id=&cityCode=&product=

- _请求参数_
- 经测试：定位参数中经纬度为必传，city固定为字符串数组

| 字段    | 类型               | 解释                              |
|:------|:-----------------|:--------------------------------|
| act    | string           | 请求行为。固定`CustomerList`           |
| id    | string           | 默认`0`                           |
| product    | string           | 分类id(不确定是否可以使用一级分类id)。默认`0`获取全部 |
| cityCode    | number           | 城市代码。默认`0`                      |
| city    | array<strnig>长度3 | 城市。没有也必须给空字符串的数组                |
| lat    | string           | 纬度 。必填                          |
| lng    | string           | 经度 。必填                              |

- _响应参数_

| 字段           | 类型     | 解释                                                                        |
|:-------------|:-------|:--------------------------------------------------------------------------|
| id           | number | id                                                                        |
| cname        | string | 医院名称                                                                      |
| addr         | string | 地址1                                                                       |
| addr2        | string | 地址2                                                                       |
| BigPic       | string | 大图                                                                        |
| SmallPic     | string | 小图                                                                        |
| province     | number | 省级城市代码                                                                    |
| city         | number | 市级城市代码                                                                    |
| county       | number | 县级城市代码                                                                    |
| distance     | number | 距离(单位/km)                                                                 |
| DistanceShow | number | 距离显示。默认0                                                                  |
| lat          | number | 纬度                                                                        |
| lng          | number | 经度                                                                        |
| notice       | string | 备注                                                                        |
| IdcardLimit  | string | 身份限制                                                                      |
| PayMent      | Object | 支付方式: {"alipay":"true","WechatPay":"true","UnionPay":"","cashier":"true"} |
| sort         | number | 排序方式。默认1                                                                  |
| tags         | array<string> | 标签                                                                        |
| tel          | string | 联系电话                                                                      |


### 获取医院详情
> GET /sc/wx/HandlerSubscribe.ashx?act=CustomerProduct&id=&lat=&lng=

- _请求参数_

| 字段    | 类型     | 解释                              |
|:------|:-------|:--------------------------------|
| act    | string | 请求行为。固定`CustomerProduct`        |
| id    | number | 医院id                            |
| lat    | string | 纬度                              |
| lng    | string | 经度                              |

- _响应参数_

| 字段           | 类型             | 解释                                                                        |
|:-------------|:---------------|:--------------------------------------------------------------------------|
| cname        | string         | 医院名称                                                                      |
| list         | array<Product> | 疫苗                                                                        |
| addr         | string         | 地址1                                                                       |
| addr2        | string         | 地址2                                                                       |
| BigPic       | string         | 大图                                                                        |
| distance     | number         | 距离(单位/km)                                                                 |
| DistanceShow | string         | 距离显示。默认0                                                                  |
| lat          | number         | 纬度                                                                        |
| lng          | number         | 经度                                                                        |
| notice       | string         | 备注                                                                        |
| IdcardLimit  | string         | 身份限制                                                                      |
| PayMent      | Object         | 支付方式: {"alipay":"true","WechatPay":"true","UnionPay":"","cashier":"true"} |
| tel          | string         | 联系电话                                                                      |
| status       | number         | 状态码。200应该是正常的                                                             |

- Product

| 字段              | 类型                 | 解释            |
|:----------------|:-------------------|:--------------|
| id              | number             | 产品id          |
| BtnLabel        | string             | 按钮文字。可以判断是否开始 |
| date            | date、string        | 默认`暂无`        |
| descript        | string             | 描述            |
| enable          | boolean            | 是否开始          |
| price           | number             | 价格(单位/元)      |
| questionnaireId | number             | 提问id。默认0      |
| remarks         | string             | 备注            |
| text            | string             | 产品名称          |
| warn            | string             | 警告            |
| NumbersVaccine  | array<NumbersVaccine> | 次数            |
| tags            | array<string>      | 标签            |

- NumbersVaccine

| 字段    | 类型     | 解释   |
|:------|:-------|:-----|
| cname | string | 名称   |
| value | number | 值    |


### 获取接种时间
> GET /sc/wx/HandlerSubscribe.ashx?act=GetCustSubscribeDateAll&pid=&id=&month=
- _请求参数_

| 字段    | 类型     | 解释                               |
|:------|:-------|:---------------------------------|
| act    | string | 请求行为。固定`GetCustSubscribeDateAll` |
| id    | number | 医院id                             |
| pid    | number | 产品id                             |
| month    | string | 月。格式：`yyyyMM`                    |

- _响应参数_
- > 外层格式: { list: [], status: 200 }

| 字段     | 类型      | 解释                |
|:-------|:--------|:------------------|
| date   | string  | 日期。如：`2021-12-31` |
| enable | boolean | 是否可预约             |





### 获取接种预约详情
> GET /sc/wx/HandlerSubscribe.ashx?act=GetCustSubscribeDateDetail&pid=&id=&scdate=
- _请求参数_

| 字段    | 类型     | 解释                                  |
|:------|:-------|:------------------------------------|
| act    | string | 请求行为。固定`GetCustSubscribeDateDetail` |
| id    | number | 医院id                                |
| pid    | number | 产品id                                |
| scdate    | string | 接种时间。格式：`yyyy-MM-dd`                |

- _响应参数_
- > 返回数据需要解密

| 字段     | 类型      | 解释                |
|:-------|:--------|:------------------|
| enable | boolean | 是否可预约             |

### 获取验证码
> GET /sc/wx/HandlerSubscribe.ashx?act=GetCaptcha&mxid=qR8nAfebAADJiDQB
- _请求参数_

| 字段    | 类型     | 解释                  |
|:------|:-------|:--------------------|
| act    | string | 请求行为。固定`GetCaptcha` |
| mxid    | string | qR8nAfebAADJiDQB    |

- _响应参数_

| 字段     | 类型      | 解释   |
|:-------|:--------|:-----|
| ignore | boolean | 是否忽略 |
| status | number  | 200  |


### 提交预约信息
> POST /sc/api/User/OrderPost
> Cookie: ASP.NET_SessionId=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpYXQiOjE2NDA5NDA3MjYuNDYzNTkzNSwiZXhwIjoxNjQwOTQ0MzI2LjQ2MzU5MzUsInN1YiI6IllOVy5WSVAiLCJqdGkiOiIyMDIxMTIzMTA0NTIwNiIsInZhbCI6InFSOG5BUUlBQUFBUU1tVmpNemszWXpGa05qTTFZVGt3TlJ4dmNYSTFielZJZVdWMk9VZFZhVXN0YW1jNFNVZFdWMmhKYlVabkFCeHZcclxuVlRJMldIUTFPR05UWVVnNFJrNUtjSHA0ZFdvd1dETkhZakZKRGpFM01TNHlNVEl1TWpReUxqRTFBQkJ4VWpodVFXWmxZa0ZCUkVwcFxyXG5SRkZDQVFBQUFBQT0ifQ.ZhZoTpxU-mYd-v6D5S1rz-z8XNggV03-pXufdJ5jq_M; path=/

- _请求参数_
- > 请求数据需要加密

| 字段    | 类型     | 解释                  |
|:------|:-------|:--------------------|
| act    | string | 请求行为。固定`GetCaptcha` |
| mxid    | string | 不清楚                 |

- _响应参数_

| 字段     | 类型     | 解释   |
|:-------|:-------|:-----|
| msg    | string | 提交成功 |
| status | number | 200  |


### 查询预约状态
> GET /sc/wx/HandlerSubscribe.ashx?act=GetOrderStatus

- _请求参数_
- > 请求数据需要加密

| 字段    | 类型     | 解释                      |
|:------|:-------|:------------------------|
| act    | string | 请求行为。固定`GetOrderStatus` |
| mxid    | string | 不清楚                     |

- _响应参数_

| 字段      | 类型     | 解释                |
|:--------|:-------|:------------------|
| status  | number | 408。预约成功。不清楚具体怎么用 |
| Fnumber | string | `21123821`。不清楚具体怎么用         |
| msg    | string |                   |


### 查询预约
> GET /sc/wx/HandlerSubscribe.ashx?act=UserSubcribeList

- _请求参数_

| 字段    | 类型     | 解释                        |
|:------|:-------|:--------------------------|
| act    | string | 请求行为。固定`UserSubcribeList` |

- _响应参数_
- > 外层格式: { list: [], status: 200 }

| 字段          | 类型      | 解释                          |
|:------------|:--------|:----------------------------|
| id          | string  |                             |
| cdate       | string  | 订单创建时间                      |
| cname       | string  | 医院名称                        |
| EndTime     | string  | 1900-01-01 11:30 不清楚        |
| finishedBtn | boolean |                             |
| Fnumber     | string  | 可能是订单号                      |
| isCancel    | boolean | 是否取消                        |
| isCancel    | boolean | 是否完成                        |
| mxid        | string  | 39927 可能是预约号                |
| product     | string  | 产品名称                        |
| username    | string  | 预约人名称                       |
| VaccineDate | date    | 2022-01-05 08:30-11:30 接种时间 |
| vdate       | date    | 2022-01-05 00:00 不清楚        |
