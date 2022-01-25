var t, a = require("../../../../3F1067E7AB6B1EDF59760FE01FCEBD94.js"), s = require("../../../../F4E45AF2AB6B1EDF928232F5F96EBD94.js"), e = getApp(), i = 0, o = 0, n = 0, c = 0;

Page({
    data: {
        times: 1,
        userInfo: "",
        subscribeInfo: "",
        idcardNsy: !1,
        reloadVisible: !1,
        headtipVisible: !0,
        isTask: !1,
        captchaVisible: !1,
        guid: "",
        moveCaptcha: {}
    },
    makeDelay: function(t) {
        return 100 * Math.ceil(10 * Math.random()) + t;
    },
    onLoad: function(a) {
        e.globalData.subscribeInfo.customerName = a.customer, e.globalData.subscribeInfo.mxid = a.mxid, 
        e.globalData.subscribeInfo.stime = a.stime, e.globalData.subscribeInfo.etime = a.etime, 
        e.globalData.subscribeInfo.subscribeTime = a.date, this.setData({
            subscribeInfo: e.globalData.subscribeInfo
        }), t = setInterval(this.task, 100);
    },
    onShow: function() {
        getApp().dot(), this.setUserinfo(), this.setData({
            idcardNsy: e.globalData.subscribeInfo.IdcardLimit
        });
    },
    onUnload: function() {
        t && (clearInterval(t), t = null), this.resetTask();
    },
    task: function() {
        c > 1 && n >= c && (n = 0, c = 0, this.getOrderStatus()), c > 1 && (n += 100), o > 1 && i >= o && (i = 0, 
        o = 0, this.doSubmit()), o > 1 && (i += 100);
    },
    closeHeadtip: function() {
        this.setData({
            headtipVisible: !1
        });
    },
    setUserinfo: function() {
        var t = this;
        e.globalData.userInfo ? this.setData({
            userInfo: e.globalData.userInfo
        }) : e.getUserInfo(function(a) {
            a ? (t.setData({
                userInfo: a,
                reloadVisible: !1
            }), e.globalData.userInfo = a) : wx.showModal({
                title: "完善信息",
                content: "需填写个人信息，才能预约",
                showCancel: !1,
                success: function(t) {
                    wx.navigateTo({
                        url: "/packages/mine/profile/profile"
                    });
                }
            });
        }, function(a) {
            t.setData({
                reloadVisible: !0
            });
        });
    },
    chooseTimes: function(t) {
        var a = t.currentTarget.dataset.times;
        this.setData({
            times: a
        });
    },
    jump: function(t) {
        var a = t.currentTarget.dataset.url;
        -1 != a.indexOf("tabs") ? wx.switchTab({
            url: a
        }) : wx.navigateTo({
            url: a
        });
    },
    redirectTo: function(t) {
        var a = t.currentTarget.dataset.url;
        wx.redirectTo({
            url: a
        });
    },
    submit: function() {
        this.setData({
            guid: ""
        });
        var t = this.data.userInfo;
        "true" != this.data.idcardNsy || t.idcard ? !t.cname || t.cname.length < 2 || !t.birthday ? wx.showModal({
            title: "提交失败",
            content: "需完善个人信息",
            showCancel: !1
        }) : this.data.guid || this.reflushMovCaptcha() : wx.showModal({
            title: "提交失败",
            content: "需完善证件号码",
            showCancel: !1
        });
    },
    doSubmit: function() {
        var t = this, s = this.data.subscribeInfo, e = this.data.userInfo;
        a.submitScribe({
            birthday: e.birthday,
            tel: e.tel,
            sex: e.sex,
            cname: e.cname,
            doctype: e.doctype,
            idcard: e.idcard,
            mxid: s.mxid,
            date: s.subscribeTime,
            pid: s.pid,
            Ftime: this.data.times,
            guid: this.data.guid
        }, function(a) {
            200 == a.data.status || 600 == a.data.status ? c = t.makeDelay(1e3) : (t.setData({
                isTask: !1
            }), wx.showModal({
                title: "预约失败",
                content: a.data.msg,
                showCancel: !1
            }));
        }, function() {
            t.resetTask();
        });
    },
    getOrderStatus: function() {
        var t = this;
        a.getOrderStatus(function(a) {
            c = 0, n = 0, 200 == a.data.status ? (t.setData({
                isTask: !1
            }), wx.redirectTo({
                url: "/packages/pov/pages/success/success"
            })) : 300 == a.data.status ? (t.setData({
                isTask: !1
            }), wx.showModal({
                title: "预约失败，预约已存在",
                content: "点击【我的预约】进行查看",
                cancelText: "稍后查看",
                confirmText: "我的预约",
                success: function(t) {
                    t.confirm && wx.redirectTo({
                        url: "/packages/mine/subscribeList/subscribeList"
                    });
                }
            })) : 0 == a.data.status ? c = t.makeDelay(1e3) : (t.setData({
                isTask: !1
            }), wx.showModal({
                title: "预约失败",
                content: a.data.msg,
                showCancel: !1
            }));
        });
    },
    resetTask: function() {
        i = 0, o = 0, c = 0, n = 0;
    },
    reflushMovCaptcha: function() {
        var t = this;
        a.get({
            url: s.getMovCaptcha,
            data: {
                mxid: this.data.subscribeInfo.mxid
            },
            method: "get",
            success: function(a) {
                if (201 != a.data.status) return a.data.ignore ? (t.setData({
                    isTask: !0,
                    captchaVisible: !1,
                    moveCaptcha: ""
                }), t.resetTask(), void (o = t.makeDelay(0))) : void t.setData({
                    captchaVisible: !0,
                    moveCaptcha: a.data
                });
                wx.showModal({
                    title: a.data.msg,
                    showCancel: !1,
                    success: function(t) {
                        wx.navigateBack();
                    }
                });
            },
            fail: function() {}
        });
    },
    bindMovableShow: function() {
        this.setData({
            captchaVisible: !1
        });
    },
    bindMovableResult: function(t) {
        var a = this;
        t.detail.success && this.setData({
            captchaVisible: !1,
            guid: t.detail.data
        }, function() {
            a.setData({
                isTask: !0
            }), a.resetTask(), o = a.makeDelay(0);
        });
    }
});