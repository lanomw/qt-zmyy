var t = require("../../../../3F1067E7AB6B1EDF59760FE01FCEBD94.js"), a = getApp();

Page({
    data: {
        cdcInfo: "",
        officialVisible: !0,
        reloadVisible: !1,
        questionnaireId: "",
        noticeHtml: "",
        isRgister: !1,
        userInfo: ""
    },
    onLoad: function(t) {
        t.id ? a.globalData.subscribeInfo.customerId = decodeURIComponent(t.id) : wx.navigateBack();
    },
    userLogin: function(t) {
        var i = this, o = a.globalData.userInfo;
        if (o) return this.setData({
            userInfo: o
        }), t(o);
        a.getUserInfo(function(a) {
            a ? (i.setData({
                userInfo: a
            }), t(a)) : wx.showModal({
                title: "完善信息",
                content: "需填写个人信息，才能预约",
                showCancel: !1,
                success: function(t) {
                    wx.navigateTo({
                        url: "/packages/mine/profile/profile"
                    });
                }
            });
        }, function(t) {
            i.setData({
                reloadVisible: !0
            });
        });
    },
    onShow: function() {
        var t = this;
        getApp().dot(), this.setData({
            questionnaireId: a.globalData.questionnaireFlag
        }), this.data.userInfo || this.userLogin(function(a) {
            t.reload();
        });
    },
    reload: function() {
        var i = this;
        if (this.setData({
            reloadVisible: !1
        }), a.globalData.questionnaireFlag = 0, a.globalData.personPosition) var o = {
            id: a.globalData.subscribeInfo.customerId,
            lat: a.globalData.personPosition.lat,
            lng: a.globalData.personPosition.lng
        }; else o = {
            id: a.globalData.subscribeInfo.customerId
        };
        t.getCDCDetail(o, function(t) {
            if (200 == t.data.status) {
                if (t.data.distance = t.data.distance ? Math.floor(t.data.distance) : 0, a.globalData.subscribeInfo.customerName = t.data.cname,
                t.data.startDate && -1 != t.data.startDate.indexOf(":")) {
                    var o = t.data.startDate.split(" "), e = o[1].split(":");
                    e.pop(), t.data.startDate = o[0] + " " + e.join(":");
                }
                if (t.data.endDate && -1 != t.data.endDate.indexOf(":")) {
                    var n = t.data.endDate.split(" "), s = n[1].split(":");
                    s.pop(), t.data.endDate = n[0] + " " + s.join(":");
                }
                null != t.data.BigPic && null != t.data.BigPic.indexOf && -1 == t.data.BigPic.indexOf("none") || (t.data.BigPic = "/commons/images/none.png"),
                i.showPovNotice(t, !0);
            }
            wx.stopPullDownRefresh();
        }, function(t) {
            i.setData({
                reloadVisible: !0
            }), wx.stopPullDownRefresh();
        });
    },
    showPovNotice: function(t, a) {
        if (a) {
            var i = wx.getStorageSync("zfsw_povNotice"), o = "povNotice_".concat(t.data.lat, "_").concat(t.data.lng);
            if (i && i.length > 0) {
                if ((i = JSON.parse(i))[o]) return void this.setData({
                    cdcInfo: t.data
                });
            } else i = {};
            i[o] = !0, wx.setStorageSync("zfsw_povNotice", JSON.stringify(i)), this.setData({
                cdcInfo: t.data,
                noticeHtml: t.data.notice
            });
        } else this.setData({
            noticeHtml: this.data.cdcInfo.notice
        });
    },
    getPosition: function(t) {
        var i = this;
        Boolean(a.globalData.personPosition) ? t(a.globalData.personPosition) : wx.getSetting({
            success: function(o) {
                "getSetting:ok" != o.errMsg || Boolean(o.authSetting) && null != o.authSetting["scope.userLocation"] && 1 != o.authSetting["scope.userLocation"] ? i.getPositionFaild() : (wx.showLoading({
                    title: "定位加载中..."
                }), wx.getLocation({
                    type: "gcj02",
                    success: function(i) {
                        a.globalData.personPosition = {
                            lat: i.latitude,
                            lng: i.longitude
                        }, t(a.globalData.personPosition);
                    },
                    fail: function(t) {
                        wx.hideLoading(), i.getPositionFaild();
                    }
                }));
            },
            fail: function(t) {
                i.getPositionFaild();
            }
        });
    },
    getPositionFaild: function() {
        this.setData({
            curpos: "定位失败"
        }, function() {
            a.globalData.positionInfo.status = -1;
        });
    },
    jump: function(t) {
        var i = t.currentTarget.dataset.url;
        -1 != i.indexOf("tabs") ? wx.switchTab({
            url: i
        }) : (this.data.cdcInfo && this.data.cdcInfo.list && this.data.cdcInfo.list[t.currentTarget.dataset.pindex] && (a.globalData.subscribeInfo.NumbersVaccine = this.data.cdcInfo.list[t.currentTarget.dataset.pindex].NumbersVaccine),
        wx.navigateTo({
            url: i
        }));
    },
    toMap: function(t) {
        var a = this.data.cdcInfo.lat, i = this.data.cdcInfo.lng;
        wx.openLocation({
            latitude: a,
            longitude: i,
            name: this.data.cdcInfo.cname,
            address: this.data.cdcInfo.addr,
            fail: function(t) {
                wx.showToast({
                    title: t,
                    duration: 1500
                });
            }
        });
    },
    call: function(t) {
        wx.showModal({
            title: "提示",
            content: "是否拨打电话",
            success: function(a) {
                a.confirm && wx.makePhoneCall({
                    phoneNumber: t.currentTarget.dataset.tel,
                    fail: function(t) {
                        wx.showToast({
                            title: t,
                            duration: 1500
                        });
                    }
                });
            }
        });
    },
    officialErr: function(t) {
        0 != t.status ? this.setData({
            officialVisible: !1
        }) : this.setData({
            officialVisible: !0
        });
    },
    onShareAppMessage: function() {
        return {
            title: this.data.cdcInfo.cname,
            imageUrl: this.data.cdcInfo.BigPic,
            path: "/packages/pov/pages/vaccinList/vaccinList?id=" + a.globalData.subscribeInfo.customerId
        };
    },
    onPullDownRefresh: function() {
        this.reload();
    },
    loaderror: function() {
        var t = this.data.cdcInfo;
        t.BigPic = "/commons/images/none.png", this.setData({
            cdcInfo: t
        });
    }
});
