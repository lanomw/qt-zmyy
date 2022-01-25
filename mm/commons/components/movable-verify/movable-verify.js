var t = require("../../../F4E45AF2AB6B1EDF928232F5F96EBD94.js"), a = require("../../../3F1067E7AB6B1EDF59760FE01FCEBD94.js");

Component({
    properties: {
        captcha: {
            type: Object,
            value: {}
        },
        show: {
            type: Boolean,
            value: !1
        }
    },
    observers: {
        show: function(t) {
            !1 === t && this.setData({
                bx: 0,
                mx: 0,
                loadingTip: !1,
                resultTipClass: "",
                direction: "horizontal"
            });
        }
    },
    data: {
        bx: 0,
        mx: 0,
        loadingTip: !1,
        resultTipClass: "",
        direction: "horizontal"
    },
    methods: {
        close: function() {
            this.triggerEvent("visible", !1);
        },
        bindchange: function(t) {
            var a = Math.floor(t.detail.x);
            "none" != this.data.direction && (this.data.loadingTip ? this.setData({
                bx: a,
                loadingTip: !1,
                resultTipClass: ""
            }) : this.setData({
                bx: a
            }));
        },
        reflush: function() {
            this.setData({
                bx: 0,
                mx: 0,
                loadingTip: "",
                resultTipClass: "",
                direction: "horizontal"
            }), this.triggerEvent("reflush");
        },
        bindtouchend: function(e) {
            var i = this;
            "none" != this.data.direction ? wx.createSelectorQuery().in(this).select(".content").boundingClientRect(function(e) {
                i.setData({
                    loadingTip: "验证码校验中..."
                }), a.get({
                    url: t.putMovCaptcha,
                    method: "get",
                    data: {
                        token: i.properties.captcha.payload.SecretKey || "",
                        x: Math.floor(310 * i.data.bx / e.width),
                        y: 5,
                        mxid: getApp().globalData.subscribeInfo.mxid
                    },
                    fail: function() {},
                    success: function(t) {
                        i.setData({
                            loadingTip: 201 == t.data.status ? "验证失败" : t.data.msg,
                            resultTipClass: 200 != t.data.status ? "error" : "success",
                            direction: "none"
                        }), i.triggerEvent("result", {
                            success: 200 == t.data.status,
                            data: t.data.guid
                        });
                    }
                });
            }).exec() : this.setData({
                mx: this.data.bx
            });
        }
    }
});