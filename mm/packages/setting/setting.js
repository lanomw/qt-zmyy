var a = getApp(), t = require("../../3F1067E7AB6B1EDF59760FE01FCEBD94.js");

Page({
    data: {},
    clearStorage: function() {
        wx.removeStorage({
            key: "addr_storage",
            success: function(t) {
                a.globalData.positionInfo.status = -2, a.globalData.personPosition = !1, wx.setStorageSync("clear_search", !0), 
                wx.showToast({
                    title: "清除成功"
                });
            }
        });
    },
    cancellation: function() {
        wx.showModal({
            title: "账户注销确认",
            content: "注销后，您的个人信息与留存的其他信息将会清空且无法找回",
            confirmColor: "#283036",
            cancelColor: "#283036",
            confirmText: "再想想",
            cancelText: "确认注销",
            success: function(e) {
                e.cancel && t.setUserCancel(function(e) {
                    if (200 == e.data.status) wx.showToast({
                        title: "注销成功",
                        success: function() {
                            a.globalData.userInfo = null, a.globalData.hasUserInfo = !1, wx.setStorageSync("clear_search", !0), 
                            t.setSessionId(null), wx.reLaunch({
                                url: "/tabs/tab_1/tab_1"
                            });
                        }
                    }); else {
                        var s = "注销失败，请稍后再试";
                        e.data && e.data.msg && (s = e.data.msg), wx.showModal({
                            title: "请求失败",
                            content: s,
                            showCancel: !1
                        });
                    }
                });
            }
        });
    }
});