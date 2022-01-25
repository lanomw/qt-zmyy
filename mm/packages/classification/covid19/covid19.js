var t, a = require("../../../@babel/runtime/helpers/defineProperty"), e = require("../../../3F1067E7AB6B1EDF59760FE01FCEBD94.js"), o = getApp(), i = require("../../../4CEE3912AB6B1EDF2A88511523CEBD94.js"), s = require("../../../A6F6C5F0AB6B1EDFC090ADF727BEBD94.js");

Page((a(t = {
    data: {
        positionFailed: !1,
        reloadVisible: !1,
        reload: !1,
        lat: 0,
        lng: 0,
        search: "",
        cityCode: "",
        region: [ "", "城市", "" ],
        povlist: "",
        name: "定位中...",
        searchTip: !0,
        isFocus: !1,
        customerid: 0,
        product: 0,
        gestureStime: 0,
        gestureSlen: 0,
        gestureDirect: "",
        tmVisable: !1,
        isSubScribe: !0,
        ischoose: !1,
        productSticky: !1,
        productStickyScrolltop: ""
    },
    onLoad: function(t) {
        this.setData({
            customerid: t.id ? decodeURIComponent(t.id) : 0
        }), this.reload();
    },
    onShow: function() {
        getApp().dot();
    },
    setChooseCity: function() {
        var t = this, a = o.globalData.personPosition, e = o.globalData.positionInfo;
        this.setData({
            lat: a.lat || e.lat,
            lng: a.lng || e.lng,
            region: e.region,
            name: e.name,
            cityCode: e.code
        }, function() {
            t.loadData();
        });
    },
    reload: function() {
        o.globalData.positionInfo.status = -2, this.data.reloadVisible && this.setData({
            reloadVisible: !1
        }), wx.removeStorageSync("clear_search"), this.getVaccines(), this.setPosition();
    }
}, "onShow", function() {
    var t = this;
    wx.getStorage({
        key: "clear_search",
        success: function(a) {
            a.data && wx.clearStorage({
                key: "clear_search",
                success: function(a) {
                    t.setData({
                        search: ""
                    }), t.setPosition();
                }
            });
        },
        fail: function(a) {
            s.info("获取缓存失败"), s.setFilterMsg("clear_search"), t.data.reload && (t.setPosition(), 
            t.setData({
                reload: !1
            }));
        }
    });
}), a(t, "getVaccines", function() {
    var t = this;
    e.getVaccines(function(a) {
        200 == a.data.status && a.data.list.length > 0 && (a.data.list.unshift({
            id: 0,
            cname: "全部"
        }), t.setData({
            pList: a.data.list
        }));
    }, function(a) {
        t.setData({
            reloadVisible: !0
        });
    });
}), a(t, "setPosition", function() {
    var t = o.globalData.positionInfo, a = o.globalData.personPosition, e = this;
    t.status < 1 ? wx.getSetting({
        success: function(t) {
            if ("getSetting:ok" != t.errMsg || Boolean(t.authSetting) && null != t.authSetting["scope.userLocation"] && 1 != t.authSetting["scope.userLocation"]) wx.showModal({
                title: "权限提示",
                content: "请允许定位权限",
                confirmText: "去设置",
                showCancel: !1,
                success: function(t) {
                    e.setData({
                        reload: !0
                    }), wx.openSetting();
                }
            }); else {
                var a = wx.getStorageSync("addr_storage");
                a ? e.setPositionData(a) : e.getLoaction();
            }
        },
        fail: function(t) {
            e.getPositionFaild();
        }
    }) : (a.code == t.code ? this.setData({
        ischoose: !1
    }) : this.setData({
        ischoose: !0
    }), this.setChooseCity());
}), a(t, "setPositionData", function(t) {
    var a = 1e4 * t.result.ad_info.adcode.slice(0, 2), e = t.result.ad_info.district;
    o.globalData.positionInfo.name = e, o.globalData.positionInfo.status = 1, o.globalData.positionInfo.code = t.result.ad_info.adcode, 
    o.globalData.positionInfo.pCode = a, o.globalData.personPosition = {
        lat: t.latitude,
        lng: t.longitude,
        name: e,
        code: t.result.ad_info.adcode,
        pCode: a,
        status: 1
    }, t.expire_time = Date.parse(new Date()) + 72e5, wx.setStorageSync("addr_storage", t), 
    this.setChooseCity();
}), a(t, "getPositionFaild", function() {
    this.setData({
        name: "定位失败",
        positionFailed: !0
    }, function() {
        o.globalData.positionInfo.status = -1;
    });
}), a(t, "getLoaction", function() {
    var t = this;
    Boolean(o.globalData.personPosition) ? (o.globalData.positionInfo.status = 1, t.setData({
        lat: o.globalData.personPosition.lat,
        lng: o.globalData.personPosition.lng,
        name: o.globalData.positionInfo.name,
        positionFailed: !1
    }), t.loadData()) : (wx.showLoading({
        title: "定位加载中..."
    }), wx.getLocation({
        type: "gcj02",
        success: function(a) {
            console.log(a), wx.hideLoading(), wx.getStorage({
                key: "addr_storage",
                success: function(e) {
                    "getStorage:ok" == e.errMsg && e.data.latitude && e.data.expire_time && e.data.expire_time - Date.parse(new Date()) > 0 && e.data.latitude == a.latitude && e.data.longitude == a.longitude ? t.setPositionData(e.data) : t.reverseGeocoder(a.latitude, a.longitude);
                },
                fail: function() {
                    t.reverseGeocoder(a.latitude, a.longitude);
                }
            });
        },
        fail: function(a) {
            wx.hideLoading(), t.getPositionFaild();
        }
    }));
}), a(t, "reverseGeocoder", function(t, a) {
    var e = this;
    i.ReverseGeocoder(t, a).then(function(t) {
        e.setPositionData(t);
    }, function(t) {
        e.setData({
            isPosition: -1,
            name: "选择城市",
            povlist: []
        });
    });
}), a(t, "loadData", function() {
    var t = this, a = this;
    a.setData({
        povlist: ""
    });
    var o = a.data.region, i = this.data.cityCode;
    this.data.ischoose || (o = [ "", "", "" ], i = 0);
    var s = {
        city: o,
        lat: a.data.lat,
        lng: a.data.lng,
        cityCode: i
    };
    e.getCovid19CDC(s, function(t) {
        if (200 == t.data.status) {
            var e = [];
            t.data.list.forEach(function(t) {
                t.tags && t.tags.indexOf && -1 != t.tags.indexOf("可预约") ? t.isSubScribe = !0 : t.isSubScribe = !1, 
                e.push(t);
            }), a.setData({
                povlist: e
            });
        }
    }, function(a) {
        t.setData({
            reloadVisible: !0
        });
    });
}), a(t, "jump", function(t) {
    wx.navigateTo({
        url: t.currentTarget.dataset.url
    });
}), a(t, "search", function() {
    this.setData({
        isFocus: !0
    });
}), a(t, "onsearch", function(t) {
    var a = t.detail.value, e = this.data.povlist;
    for (var o in e) e[o].cname.indexOf(a) < 0 ? e[o].hide = !0 : e[o].hide = !1;
    this.setData({
        povlist: e,
        search: a,
        searchTip: a.length < 1
    });
}), a(t, "productChange", function(t) {
    this.data.tmVisable ? this.setData({
        product: t.detail,
        tmVisable: !1
    }) : this.setData({
        product: t.detail
    }), this.loadData();
}), a(t, "tmchange", function(t) {
    this.setData({
        tmVisable: "false" != t.detail
    });
}), a(t, "pagescroll", function(t) {
    var a = this.data.scrolltopVisible;
    !a && t.detail.scrollTop > 70 ? this.setData({
        scrolltopVisible: !0
    }) : a && t.detail.scrollTop <= 80 && this.setData({
        scrolltopVisible: !1
    });
    var e = this.data.productSticky, o = this.data.productStickyScrolltop;
    !e && t.detail.scrollTop >= o ? this.setData({
        productSticky: !0
    }) : e && t.detail.scrollTop < o && this.setData({
        productSticky: !1
    });
}), a(t, "pagescrolltotop", function() {
    this.setData({
        pagescrolltop: "scrolltop",
        scrolltopVisible: !1
    });
}), t));