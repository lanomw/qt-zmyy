var e = getApp();

module.exports = {
    IdcardVerdify: function(e) {
        return function(e) {
            var t = "", n = !0;
            if (e && /^\d{6}(18|19|20)?\d{2}(0[1-9]|1[012])(0[1-9]|[12]\d|3[01])\d{3}(\d|X)$/i.test(e)) if ({
                11: "北京",
                12: "天津",
                13: "河北",
                14: "山西",
                15: "内蒙古",
                21: "辽宁",
                22: "吉林",
                23: "黑龙江 ",
                31: "上海",
                32: "江苏",
                33: "浙江",
                34: "安徽",
                35: "福建",
                36: "江西",
                37: "山东",
                41: "河南",
                42: "湖北 ",
                43: "湖南",
                44: "广东",
                45: "广西",
                46: "海南",
                50: "重庆",
                51: "四川",
                52: "贵州",
                53: "云南",
                54: "西藏 ",
                61: "陕西",
                62: "甘肃",
                63: "青海",
                64: "宁夏",
                65: "新疆",
                71: "台湾",
                81: "香港",
                82: "澳门",
                91: "国外 "
            }[e.substr(0, 2)]) {
                if (18 == e.length) {
                    e = e.split("");
                    for (var r = [ 7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2 ], s = [ 1, 0, "X", 9, 8, 7, 6, 5, 4, 3, 2 ], u = 0, o = 0; o < 17; o++) u += e[o] * r[o];
                    s[u % 11] != e[17] && (t = "校验位错误", n = !1);
                }
            } else t = "地址编码错误", n = !1; else t = "身份证号格式错误", n = !1;
            return n || console.log(t), n;
        }(e);
    },
    Trim: function(e) {
        return e.replace(/(^\s*)|(\s*$)/g, "");
    },
    IdcardBirthday: function(e) {
        var t = "";
        return null != e && "" != e && (15 == e.length ? t = "19" + e.substr(6, 6) : 18 == e.length && (t = e.substr(6, 8)), 
        t = t.replace(/(.{4})(.{2})/, "$1-$2-")), t;
    },
    IsPhone: function(e) {
        return !!/^[1][3,4,5,6,7,8,9][0-9]{9}$/.test(e);
    },
    wxp: function(e) {
        return function(t) {
            return new Promise(function(n, r) {
                t.success = function(e) {
                    n(e);
                }, t.fail = function(e) {
                    r(e);
                }, e(t);
            });
        };
    },
    auth: function(t) {
        "" == getApp().globalData.header.Cookie ? (wx.showLoading({
            title: "数据加载中"
        }), wx.login({
            success: function(n) {
                wx.request({
                    url: e.globalData.url + "/wx/HandlerSubscribe.ashx?act=auth",
                    data: {
                        code: n.code
                    },
                    header: {
                        "content-type": "application/json"
                    },
                    success: function(e) {
                        wx.hideLoading(), getApp().globalData.header.Cookie = "ASP.NET_SessionId=" + e.data.sessionId, 
                        200 == e.data.status ? t.success() : wx.redirectTo({
                            url: "../person/person"
                        });
                    }
                });
            }
        })) : t.success();
    },
    isCnn: function(e) {
        return !!new RegExp("[\\u4E00-\\u9FFF]+", "g").test(e);
    },
    countAge: function(e) {
        var t = new Date(), n = t.getFullYear(), r = t.getMonth + 1, s = t.getDate(), u = e.split("-"), o = n - Number(u[0]);
        return (r < Number(u[1]) || s < Number(u[2])) && (o -= 1), o;
    },
    number2str: function(e) {
        return e < 10 ? "0" + e : e + "";
    },
    passportVerify: function(e) {
        return /[\w]{5,17}/.test(e);
    }
};