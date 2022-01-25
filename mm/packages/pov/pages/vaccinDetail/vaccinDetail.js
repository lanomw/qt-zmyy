var t = require("../../../../3F1067E7AB6B1EDF59760FE01FCEBD94.js"), a = getApp();

Page({
    data: {
        albumStyle: "",
        cdcInfo: {}
    },
    onLoad: function(t) {
        t.id && (a.globalData.subscribeInfo.customerId = decodeURIComponent(t.id)), this.getPovDetail();
    },
    onShareAppMessage: function() {
        return {
            title: this.data.cdcInfo.cname,
            imageUrl: this.data.cdcInfo.BigPic,
            path: "/packages/pov/pages/vaccinList/vaccinList?id=" + a.globalData.subscribeInfo.customerId
        };
    },
    getPovDetail: function() {
        var i = this, n = getApp().globalData.subscribeInfo.customerId;
        t.getPovDetail(n, function(t) {
            null != t.data.BigPic && null != t.data.BigPic.indexOf && -1 == t.data.BigPic.indexOf("none") || (t.data.BigPic = "/commons/images/none.png"), 
            a.globalData.subscribeInfo.notice = t.data.notice, i.setData({
                cdcInfo: t.data
            });
        });
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
    albumLoad: function(t) {
        var a = t.detail, i = a.height / a.width, n = Math.ceil(getApp().globalData.windowHeight * i);
        this.setData({
            albumStyle: "width: 100%; height: ".concat(n, "rpx;")
        });
    },
    loaderror: function() {
        var t = this.data.cdcInfo;
        t.BigPic = "/commons/images/none.png", this.setData({
            cdcInfo: t
        });
    },
    jump: function(t) {
        var i = t.currentTarget.dataset.url;
        if (-1 != i.indexOf("tabs")) wx.switchTab({
            url: i
        }); else {
            if (!a.globalData.subscribeInfo.notice) return void wx.showToast({
                icon: "none",
                title: "单位暂无公告"
            });
            wx.navigateTo({
                url: i
            });
        }
    }
});