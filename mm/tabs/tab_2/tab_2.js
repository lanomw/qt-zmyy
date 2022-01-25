var t = getApp(), e = require("../../3F1067E7AB6B1EDF59760FE01FCEBD94.js"), a = require("../../467620F5AB6B1EDF201048F2888EBD94.js");

Page({
    data: {
        windowHeight: t.globalData.windowHeight,
        windowWidth: t.globalData.windowWidth,
        isSuccess: !1,
        userInfo: "",
        reloadVisible: !1,
        sdkVersion: ""
    },
    onShow: function() {
        getApp().dot(), this.reload();
    },
    makeCanvasBg: function() {
        var t = wx.createCanvasContext("minebg"), e = this.data.windowWidth, a = e / 375 * 238;
        t.beginPath(), t.arc(e / 2, 2 * -a + a / 2, 2 * a, 0, 2 * Math.PI), t.setFillStyle("#5192FF"), 
        t.fill(), t.beginPath(), t.arc(e / 4, 2.2 * a, 2 * a, 0, 2 * Math.PI), t.setFillStyle("rgba(255,255,255,0.1)"), 
        t.fill(), t.beginPath(), t.arc(e, 2.2 * a, 2 * a, 0, 2 * Math.PI), t.setFillStyle("rgba(255,255,255,0.1)"), 
        t.fill();
        for (var i = 0; i < 10; i++) {
            var n = Math.ceil(10 * Math.random()) / 10;
            n = n > .3 ? .3 : n, t.beginPath(), t.arc(e * Math.random(), a * Math.random() / 8, 10 * Math.random(), 0, 2 * Math.PI), 
            t.setFillStyle("rgba(255,255,255,0.1)"), t.fill();
        }
        t.draw();
    },
    jump: function(t) {
        var e = t.currentTarget.dataset.url;
        -1 != e.indexOf("tabs") ? wx.switchTab({
            url: e
        }) : wx.navigateTo({
            url: e
        });
    },
    login: function() {
        e.getUser();
    },
    reload: function() {
        var e = this;
        t.globalData.userInfo ? this.setData({
            userInfo: t.globalData.userInfo,
            reloadVisible: !1
        }) : t.getUserInfo(function(t) {
            e.setData({
                userInfo: t,
                reloadVisible: !1
            });
        }, function(t) {
            e.setData({
                reloadVisible: !0
            });
        });
    },
    subscribe: function() {
        wx.requestSubscribeMessage({
            tmplIds: a.subscibeId,
            success: function(t) {
                "accept" == t[a.subscibeId] && wx.showToast({
                    title: "订阅成功"
                });
            },
            fail: function(t) {
                wx.showModal({
                    title: "操作提示",
                    content: "需开启【消息订阅】功能",
                    success: function(t) {
                        t.confirm && wx.openSetting();
                    }
                });
            }
        });
    },
    testAuth: function(t) {
        wx.startSoterAuthentication({
            requestAuthModes: [ "fingerPrint" ],
            challenge: "123456",
            authContent: "请用指纹解锁",
            success: function(t) {
                console.log(t);
            },
            fail: function(t) {
                console.error(t);
            }
        });
    }
});