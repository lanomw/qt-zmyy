var e = require("@babel/runtime/helpers/interopRequireDefault.js");

require("@babel/runtime/helpers/Arrayincludes.js");

var t = e(require("@babel/runtime/regenerator.js")),
    s = require("@babel/runtime/helpers/asyncToGenerator.js"),
    a = require("F4E45AF2AB6B1EDF928232F5F96EBD94.js"),
    n = require("A6F6C5F0AB6B1EDFC090ADF727BEBD94.js"),
    r = require("38DA55C1AB6B1EDF5EBC3DC63C2EBD94.js"),
    // cookie值
    i = "",
    c = 0,
    // 签名前16位作为key
    o = "",
    u = r.enc.Utf8.parse("1234567890000000"),
    // Get
    l = function () {
        var e = s(t.default.mark(function e(s) {
            var a, l;
            return t.default.wrap(function (e) {
                for (; ;) switch (e.prev = e.next) {
                    case 0:
                        !1 === s.loadingBar ? wx.showNavigationBarLoading() : 0 === s.loadingBar || wx.showLoading({
                            title: "加载中...",
                            mask: !0
                        }),
                            s.data = s.data ? s.data : {},
                            a = s.header || {
                                "content-type": "application/json"
                            },
                            l = this,
                            a.Cookie = i,
                            a.zftsl = f(),
                            wx.request({
                                url: s.url,
                                data: s.data,
                                method: "GET",
                                dataType: s.dataType || "json",
                                header: a,
                                responseType: s.responseType || "text",
                                success: function (e) {
                                    if (g(e.header), 200 == e.statusCode && p(s.url))
                                    try {
                                        if ("string" == typeof e.data) {
                                            var t = (
                                                a = e.data,
                                                    i = r.enc.Utf8.parse(o.substring(0, 16)),
                                                    c = r.enc.Hex.parse(a),
                                                    d = r.enc.Base64.stringify(c),
                                                    r.AES.decrypt(
                                                        d,
                                                        i,
                                                        {
                                                            iv: u,
                                                            mode: r.mode.CBC,
                                                            padding: r.pad.Pkcs7
                                                        }).toString(r.enc.Utf8)
                                            );
                                            t && (e.data = JSON.parse(t));
                                        }
                                    } catch (t) {
                                        n.error(t, s.url), n.setFilterMsg("AES_Err"), e.data = JSON.parse(e.data);
                                    }
                                    var a, i, c, d;
                                    408 == e.statusCode || e.data.status && 408 == e.data.status ? l.getSessionId(function (e) {
                                        l.get(s);
                                    }) : s.success(e);
                                },
                                fail: function (e) {
                                    console.error(s.url, s.data, e), n.error(e, s.url), n.setFilterMsg("request"), s.fail && s.fail(e),
                                    s.fail || wx.showModal({
                                        title: "网络繁忙",
                                        confirmText: "继续等待",
                                        success: function (e) {
                                            e.confirm && l.get(s);
                                        }
                                    });
                                },
                                complete: function (e) {
                                    if (e.header && e.header.Date) {
                                        var t = Date.parse(new Date(e.header.Date.replace(/-/g, "/")));
                                        c = t - Date.parse(new Date());
                                    }
                                    !1 === s.loadingBar ? wx.hideNavigationBarLoading() : wx.hideLoading(), s.complete;
                                }
                            });

                    case 7:
                    case "end":
                        return e.stop();
                }
            }, e, this);
        }));
        return function (t) {
            return e.apply(this, arguments);
        };
    }(),
    // POST
    d = function () {
        var e = s(t.default.mark(function e(s) {
            var a, r;
            return t.default.wrap(function (e) {
                for (; ;) switch (e.prev = e.next) {
                    case 0:
                        if (a = this, !1 === s.loadingBar ? wx.showNavigationBarLoading() : wx.showLoading({
                            title: "加载中...",
                            mask: !0
                        }), (r = s.header || {
                            "content-type": "application/json"
                        }).Cookie = i, r.zftsl = f(), !1 === s.url.includes("/api/User/OrderPost")) {
                            e.next = 9;
                            break;
                        }
                        return e.next = 8, h(JSON.stringify(s.data));

                    case 8:
                        s.data = e.sent;

                    case 9:
                        wx.request({
                            url: s.url,
                            data: s.data,
                            method: "POST",
                            dataType: s.options || "json",
                            header: r,
                            success: function (e) {
                                g(e.header), 408 == e.statusCode || e.data.status && 408 == e.data.status ? a.getSessionId(function (e) {
                                    a.post(s);
                                }) : s.success(e);
                            },
                            fail: function (e) {
                                n.error(e, s.url), n.setFilterMsg("request"), e.errMsg && -1 != e.errMsg.indexOf("timeout") ? wx.showModal({
                                    title: "网络繁忙",
                                    confirmText: "继续等待",
                                    showCancel: !1,
                                    success: function (e) {
                                        a.post(s);
                                    }
                                }) : s.fail && s.fail(e);
                            },
                            complete: function (e) {
                                if (e.header && e.header.Date) {
                                    var t = Date.parse(new Date(e.header.Date.replace(/-/g, "/")));
                                    c = t - Date.parse(new Date());
                                }
                                !1 === s.loadingBar ? wx.hideNavigationBarLoading() : wx.hideLoading(), s.complete;
                            }
                        });

                    case 10:
                    case "end":
                        return e.stop();
                }
            }, e, this);
        }));
        return function (t) {
            return e.apply(this, arguments);
        };
    }(),
    // zftsl生成
    f = function () {
        var e = (Date.parse(new Date()) + c) / 1e3 + "";
        return r.MD5("zfsw_" + e.substring(0, e.length - 1)).toString();
    },
    // Cookie保存
    g = function (e) {
        e && e["Set-Cookie"] && (i = e["Set-Cookie"],console.log('============> Cookie', i));
    };

// 加密
function h(e) {
    return new Promise(function (t, s) {
        var a = r.enc.Utf8.parse(o.substring(0, 16)),
            n = r.AES.encrypt(e, a, {
                iv: u,
                mode: r.mode.CBC,
                padding: r.pad.Pkcs7
            });
        console.log(o.substring(0, 16)),
        t(n.ciphertext.toString());
    });
}

function p(e) {
    return !1 !== e.includes("HandlerSubscribe.ashx?act=CaptchaVerify") || !1 !== e.includes("HandlerSubscribe.ashx?act=GetCustSubscribeDateDetail");
}

// 签名获取
// code需要使用原小程序生成的code
var b = function () {
    console.log('============> login ')
    var e = s(t.default.mark(function e(s, n) {
        var r, c;
        return t.default.wrap(function (e) {
            for (; ;) switch (e.prev = e.next) {
                case 0:
                    return e.next = 2, wx.login();

                case 2:
                    console.log('============> sent', e.sent, e)
                    return r = e.sent, e.next = 5, wx.getUserInfo();

                case 5:
                    c = e.sent,
                        // 获取签名前16位作为key
                        o = c.signature.substring(0, 16),
                        console.log('============> o: ', c.signature, o)
                        console.log('============> code:', r.code)
                        d({
                            // url: a.auth + "&code=".concat(r.code),
                            url: a.auth + "&code=091vsFkl22Ovs84GSJml2exKzA3vsFkj",
                            data: {
                                rawdata: c.rawData
                            },
                            success: function (e) {
                                200 == e.data.status ?
                                    (i = "ASP.NET_SessionId=" + e.data.sessionId, s(i)) :
                                    201 == e.data.status ?
                                        (i = "ASP.NET_SessionId=" + e.data.sessionId,
                                            wx.showModal({
                                                title: "完善信息",
                                                content: "需填写个人信息，才能预约",
                                                showCancel: !1,
                                                success: function () {
                                                    wx.navigateTo({
                                                        url: "/packages/mine/profile/profile"
                                                    });
                                                }
                                            })) :
                                        (console.log(e.data), wx.showModal({
                                            title: "完善信息",
                                            content: e.data.msg,
                                            showCancel: !1
                                        }));
                            },
                            fail: function (e) {
                                console.error("请求云端服务器错误：", e), n(e);
                            }
                        }, !0);

                case 8:
                case "end":
                    return e.stop();
            }
        }, e);
    }));
    return function (t, s) {
        return e.apply(this, arguments);
    };
}();

module.exports = {
    get: l,
    post: d,
    getBanner: function (e, t) {
        this.get({
            loadingBar: !1,
            url: a.getBanner,
            success: e,
            fail: t
        });
    },
    checkSessionId: function (e) {
        e(!!i);
    },
    getSessionId: b,
    getUserinfo: function (e, t) {
        this.get({
            url: a.user,
            data: {},
            success: e,
            fail: function (e) {
                console.log(e), t(e);
            }
        });
    },
    getCDC: function (e, t, s) {
        this.get({
            loadingBar: !1,
            url: a.customerList,
            data: e,
            success: t,
            fail: s
        });
    },
    getCovid19CDC: function (e, t, s) {
        this.get({
            loadingBar: !1,
            url: a.customerCov19List,
            data: e,
            success: t,
            fail: s
        });
    },
    getSubcribeList: function (e) {
        this.get({
            url: a.userSubcribeList,
            success: e
        });
    },
    putUser: function (e, t) {
        this.get({
            url: a.putUser,
            data: e,
            success: t
        });
    },
    getCDCDetail: function (e, t, s) {
        var n = "";
        Object.keys(e).forEach(function (t) {
            n += "&" + t + "=" + e[t];
        }), this.get({
            url: a.getCDCDetail + n,
            success: t,
            fail: s
        });
    },
    getVaccines: function (e, t) {
        this.get({
            loadingBar: !1,
            url: a.getVaccines,
            success: e,
            fail: t
        });
    },
    getPovDetail: function (e, t, s) {
        this.get({
            loadingBar: !1,
            url: a.getPovDetail + e,
            success: t,
            fail: s
        });
    },
    getVaccineCates: function (e, t) {
        this.get({
            url: a.getVaccineCates + e,
            success: t
        });
    },
    getSubscribeMonth: function (e, t, s) {
        var n = "";
        Object.keys(e).forEach(function (t) {
            n += "&" + t + "=" + e[t];
        }), this.get({
            url: a.getSubscribeMonth_news + n,
            success: t,
            fail: s
        });
    },
    // 订阅日期，返回数据需要解密
    getSubscribeTime: function (e, t, s) {
        var n = "";
        Object.keys(e).forEach(function (t) {
            n += "&" + t + "=" + e[t];
        }), this.get({
            url: a.getSubscribeTime + n,
            success: t,
            fail: s
        });
    },
    submitScribe: function (e, t, s) {
        this.post({
            loadingBar: 0,
            url: a.submitScribe30,
            data: e,
            success: t,
            complete: s
        });
    },
    cancelScribe: function (e, t) {
        var s = "";
        Object.keys(e).forEach(function (t) {
            s += "&" + t + "=" + e[t];
        }), this.get({
            url: a.cancelScribe + s,
            success: t
        });
    },
    getSubcribeDetail: function (e, t) {
        var s = "";
        Object.keys(e).forEach(function (t) {
            s += "&" + t + "=" + e[t];
        }), this.get({
            url: a.getSubcribeDetail + s,
            success: t
        });
    },
    getQuestionnaire: function (e, t) {
        var s = "";
        Object.keys(e).forEach(function (t) {
            s += "&" + t + "=" + e[t];
        }), this.get({
            url: a.getQuestionnaire + s,
            success: t
        });
    },
    setQuestionnaire: function (e, t) {
        this.post({
            url: a.setQuestionnaire,
            data: e,
            success: t
        });
    },
    comfirmVaccin: function (e, t) {
        this.get({
            url: a.comfirmVaccin + e.id,
            success: t
        });
    },
    getOrderStatus: function (e) {
        this.get({
            loadingBar: 0,
            url: a.getOrderStatus,
            success: e
        });
    },
    getVerifyCode: function (e) {
        this.get({
            url: a.getVerifyCode + "?t=" + Date.parse(new Date()) + "&format=base64",
            success: e
        });
    },
    getCertType: function (e) {
        this.get({
            url: a.getCertType,
            success: e
        });
    },
    getQuestionnaireCovid19: function (e) {
        this.get({
            url: a.GetCov19Question,
            success: e
        });
    },
    setQuestionnaireCovid19: function (e, t) {
        this.post({
            url: a.SetCov19Question,
            data: e,
            success: t
        });
    },
    setUserCancel: function (e) {
        this.get({
            url: a.setUserCancel,
            success: e
        });
    },
    setSessionId: function (e) {
        i = e;
    }
};
