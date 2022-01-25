var t = getApp(), a = require("libs/cityData.js");

Page({
    data: {
        lat: 0,
        lng: 0,
        search: "",
        cityCode: "",
        region: [ "", "城市", "" ],
        povlist: [],
        curpos: "定位中...",
        searchTip: !0
    },
    onLoad: function(t) {
        var a = this;
        wx.getStorage({
            key: "setDistrict",
            success: function(t) {
                Boolean(t.data) ? a.getLocalLocation(JSON.parse(t.data)) : a.getLoaction();
            },
            fail: function(t) {
                a.getLoaction();
            },
            complete: function() {
                wx.removeStorageSync("setDistrict");
            }
        });
    },
    onShow: function() {
        getApp().dot();
        var t = this;
        wx.getStorage({
            key: "setDistrict",
            success: function(a) {
                Boolean(a.data) && t.getLocalLocation(JSON.parse(a.data));
            },
            complete: function() {
                wx.removeStorageSync("setDistrict");
            }
        });
    },
    getLoaction: function() {
        var t = this;
        wx.showLoading({
            title: "定位加载中..."
        }), wx.getLocation({
            type: "gcj02",
            success: function(a) {
                var e = a.latitude, o = a.longitude;
                t.setData({
                    lat: e,
                    lng: o
                }), t.loadData(), wx.hideLoading();
            }
        });
    },
    getLocalLocation: function(t) {
        var e = this, o = a[t.pAcronym][t.pCode], i = o.name, n = "", c = "";
        Boolean(o.data) && o.data[t.acronym].forEach(function(a) {
            a.code == t.code && ("市" == a.name[a.name.length - 1] ? n = a.name : -1 != i.indexOf("重庆") || -1 != i.indexOf("北京") || -1 != i.indexOf("上海") || -1 != i.indexOf("天津") ? (n = i, 
            c = a.name) : (n = a.name, c = a.name));
        });
        e.setData({
            lat: 0,
            lng: 0,
            region: [ i, n, c ],
            curpos: c || n || i,
            cityCode: t.code
        }, function() {
            console.log(c, n, i), e.loadData();
        });
    },
    loadData: function() {
        var a = this;
        wx.showLoading({
            title: "数据加载中"
        });
        wx.request({
            url: t.globalData.url + "/wx/HandlerSubscribe.ashx?act=CustomerList",
            data: {
                city: a.data.region,
                lat: a.data.lat,
                lng: a.data.lng,
                id: a.data.customerid,
                cityCode: this.data.cityCode
            },
            header: {
                "content-type": "application/json",
                Cookie: getApp().globalData.header.Cookie
            },
            success: function(t) {
                wx.hideLoading(), 200 == t.data.status && a.setData({
                    povlist: t.data.list,
                    curpos: t.data.local ? t.data.local[2] : a.data.curpos
                });
            }
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
            searchTip: !1
        });
    },
    searchblur: function() {
        this.data.search.length < 1 && this.setData({
            searchTip: !0
        });
    },
    onsearch: function(t) {
        var a = t.detail.value, e = this.data.povlist;
        for (var o in e) e[o].cname.indexOf(a) < 0 ? e[o].hide = !0 : e[o].hide = !1;
        this.setData({
            povlist: e,
            search: a
        });
    }
});