var t = getApp(), a = require("../../3F1067E7AB6B1EDF59760FE01FCEBD94.js"), e = require("../../4CEE3912AB6B1EDF2A88511523CEBD94.js"), i = require("../../A6F6C5F0AB6B1EDFC090ADF727BEBD94.js");

Page({
    data: {
        deceptiontipVisible: !0,
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
        productStickyScrolltop: "",
        pList: [],
        cList: [],
        pActive: 0,
        cActive: 0,
        banners: [],
        bannerHeight: [],
        bannerActive: 0
    },
    onLoad: function(t) {
        this.setData({
            customerid: t.id ? decodeURIComponent(t.id) : 0
        }), this.reload(), this.getBanner(), this.makeProductStickyScrolltop();
    },
    makeProductStickyScrolltop: function() {
        var t = this, a = wx.createSelectorQuery();
        a.select("#home-product-swiper").boundingClientRect(), a.selectViewport().scrollOffset(), 
        a.exec(function(a) {
            t.setData({
                productStickyScrolltop: a[0].top
            });
        });
    },
    setChooseCity: function() {
        var a = this, e = t.globalData.personPosition, i = t.globalData.positionInfo;
        this.setData({
            lat: e.lat || i.lat,
            lng: e.lng || i.lng,
            region: i.region || [ "", "", "" ],
            name: i.name,
            cityCode: i.code
        }, function() {
            a.loadData();
        });
    },
    reload: function() {
        t.globalData.positionInfo.status = -2, this.data.reloadVisible && this.setData({
            reloadVisible: !1
        }), wx.removeStorageSync("clear_search"), this.getVaccines(), this.setPosition();
    },
    onShow: function() {
        var t = this;
        getApp().dot(), wx.getStorage({
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
                i.info("获取缓存失败"), i.setFilterMsg("clear_search"), t.data.reload && (t.setPosition(), 
                t.setData({
                    reload: !1
                }));
            }
        });
    },
    getBanner: function() {
        var t = this;
        return new Promise(function(e, i) {
            a.getBanner(function(a) {
                200 == a.data.status ? (t.setData({
                    banners: a.data.list
                }), e(!0)) : e(!1);
            }, function(t) {
                e(!1);
            });
        });
    },
    getVaccines: function() {
        var t = this;
        return new Promise(function(e, i) {
            a.getVaccines(function(a) {
                200 == a.data.status && a.data.list.length > 0 ? (a.data.list.unshift({
                    id: 0,
                    cname: "全部"
                }), t.setData({
                    pList: a.data.list
                }), e(!0)) : e(!1);
            }, function(a) {
                t.setData({
                    reloadVisible: !0
                }), e(!1);
            });
        });
    },
    setPosition: function() {
        var a = t.globalData.positionInfo, e = t.globalData.personPosition, i = this;
        a.status < 1 ? wx.getSetting({
            success: function(a) {
                if ("getSetting:ok" != a.errMsg || Boolean(a.authSetting) && null != a.authSetting["scope.userLocation"] && 1 != a.authSetting["scope.userLocation"]) wx.showModal({
                    title: "权限提示",
                    content: "请开启定位权限",
                    confirmText: "去设置",
                    showCancel: !1,
                    success: function(t) {
                        i.setData({
                            reload: !0
                        }), wx.openSetting();
                    }
                }); else {
                    var e = wx.getStorageSync("addr_storage");
                    e ? i.setPositionData(e) : (t.globalData.positionInfo.region = [ "", "", "" ], i.getLoaction());
                }
            },
            fail: function(t) {
                i.getPositionFaild();
            }
        }) : (e.code == a.code ? this.setData({
            ischoose: !1
        }) : this.setData({
            ischoose: !0
        }), this.setChooseCity());
    },
    setPositionData: function(a) {
        var e = 1e4 * a.result.ad_info.adcode.slice(0, 2), i = a.result.ad_info.district;
        t.globalData.positionInfo.name = i, t.globalData.positionInfo.status = 1, t.globalData.positionInfo.code = a.result.ad_info.adcode, 
        t.globalData.positionInfo.pCode = e, t.globalData.personPosition = {
            lat: a.latitude,
            lng: a.longitude,
            name: i,
            code: a.result.ad_info.adcode,
            pCode: e,
            status: 1
        }, a.expire_time = Date.parse(new Date()) + 72e5, wx.setStorageSync("addr_storage", a), 
        this.setChooseCity();
    },
    getPositionFaild: function() {
        this.setData({
            name: "定位失败",
            positionFailed: !0
        }, function() {
            t.globalData.positionInfo.status = -1;
        });
    },
    getLoaction: function() {
        var a = this;
        Boolean(t.globalData.personPosition) ? (t.globalData.positionInfo.status = 1, a.setData({
            lat: t.globalData.personPosition.lat,
            lng: t.globalData.personPosition.lng,
            name: t.globalData.positionInfo.name,
            positionFailed: !1
        }), a.loadData()) : (wx.showLoading({
            title: "定位加载中..."
        }), wx.getLocation({
            type: "gcj02",
            success: function(t) {
                wx.hideLoading(), wx.getStorage({
                    key: "addr_storage",
                    success: function(e) {
                        "getStorage:ok" == e.errMsg && e.data.latitude && e.data.expire_time && e.data.expire_time - Date.parse(new Date()) > 0 && e.data.latitude == t.latitude && e.data.longitude == t.longitude ? a.setPositionData(e.data) : a.reverseGeocoder(t.latitude, t.longitude);
                    },
                    fail: function() {
                        a.reverseGeocoder(t.latitude, t.longitude);
                    }
                });
            },
            fail: function(t) {
                wx.hideLoading(), a.getPositionFaild();
            }
        }));
    },
    reverseGeocoder: function(t, a) {
        var i = this;
        e.ReverseGeocoder(t, a).then(function(t) {
            i.setPositionData(t);
        }, function(t) {
            i.setData({
                isPosition: -1,
                name: "选择城市",
                povlist: []
            });
        });
    },
    loadData: function() {
        var t = this, e = this;
        e.setData({
            povlist: ""
        });
        var i = e.data.region, o = this.data.cityCode;
        this.data.ischoose || (i = [ "", "", "" ], o = 0);
        var s = {
            city: i,
            lat: e.data.lat,
            lng: e.data.lng,
            id: e.data.customerid,
            cityCode: o,
            product: this.data.product
        };
        a.getCDC(s, function(t) {
            if (200 == t.data.status) {
                var a = [];
                t.data.list.forEach(function(t) {
                    t.tags && t.tags.indexOf && -1 != t.tags.indexOf("可预约") ? t.isSubScribe = !0 : t.isSubScribe = !1, 
                    a.push(t);
                }), e.setData({
                    povlist: a
                });
            }
        }, function(a) {
            t.setData({
                reloadVisible: !0
            });
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
    search: function() {
        this.setData({
            isFocus: !0
        });
    },
    onsearch: function(t) {
        var a = t.detail.value, e = this.data.povlist;
        for (var i in e) e[i].cname.indexOf(a) < 0 ? e[i].hide = !0 : e[i].hide = !1;
        this.setData({
            povlist: e,
            search: a,
            searchTip: a.length < 1
        });
    },
    productChange: function(t) {
        this.setData({
            product: t.detail.product,
            pActive: t.detail.pActive,
            cActive: t.detail.cActive,
            cList: t.detail.cList,
            tmVisable: !1
        }), this.loadData();
    },
    tmchange: function(t) {
        this.setData({
            tmVisable: "false" != t.detail
        });
    },
    pagescroll: function(t) {
        var a = this.data.scrolltopVisible;
        !a && t.detail.scrollTop > 70 ? this.setData({
            scrolltopVisible: !0
        }) : a && t.detail.scrollTop <= 80 && this.setData({
            scrolltopVisible: !1
        });
        var e = this.data.productSticky, i = this.data.productStickyScrolltop;
        !e && t.detail.scrollTop >= i ? this.setData({
            productSticky: !0
        }) : e && t.detail.scrollTop < i && this.setData({
            productSticky: !1
        });
    },
    pagescrolltotop: function() {
        this.setData({
            pagescrolltop: "scrolltop",
            scrolltopVisible: !1,
            productSticky: !1
        });
    },
    checkcovid19: function() {
        t.getUserInfo(function(t) {
            !t.doctype || t.idcard.length < 5 ? wx.showToast({
                icon: "none",
                title: "请完善个人信息",
                success: function() {
                    wx.navigateTo({
                        url: "/packages/mine/profile/profile?idcardNsy=true&directurl=/packages/classification/covid19/covid19"
                    });
                }
            }) : wx.navigateTo({
                url: "/packages/classification/covid19/covid19"
            });
        }, function(t) {
            wx.showToast({
                icon: "none",
                title: "获取授权失败"
            });
        });
    },
    onShareAppMessage: function() {
        return {
            title: "知苗易约",
            path: "/tabs/tab_1/tab_1"
        };
    },
    bindBannerLoad: function(t) {
        var a = this.data.bannerHeight;
        a[t.currentTarget.dataset.index] = t.detail.height + 64, this.setData({
            bannerHeight: a
        });
    },
    setDeception: function() {
        this.setData({
            deceptiontipVisible: !1
        });
    }
});