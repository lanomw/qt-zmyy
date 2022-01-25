var t = require("../../../F4E45AF2AB6B1EDF928232F5F96EBD94.js"), e = require("../../../3F1067E7AB6B1EDF59760FE01FCEBD94.js");

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
            var e = Math.floor(t.detail.x / 291 * 360);
            "none" != this.data.direction && (this.data.loadingTip ? this.setData({
                bx: e,
                loadingTip: !1,
                resultTipClass: ""
            }) : this.setData({
                bx: e
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
        bindtouchend: function(a) {
            var i = this;
            "none" != this.data.direction ? wx.createSelectorQuery().in(this).select(".content").boundingClientRect(function(a) {
                i.setData({
                    loadingTip: "验证码校验中..."
                }), e.get({
                    url: t.putMovCaptcha,
                    method: "get",
                    data: {
                        token: i.properties.captcha.payload.SecretKey || "",
                        x: i.data.bx,
                        y: 5
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