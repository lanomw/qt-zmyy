var t = require("../../../../@babel/runtime/helpers/objectSpread2"), a = require("../../../../EB882611AB6B1EDF8DEE4E16E54EBD94.js"), e = (require("../../../../4CEE3912AB6B1EDF2A88511523CEBD94.js"), 
getApp());

Page({
    data: {
        isLoading: !0,
        isFocus: !1,
        searchTip: !0,
        searchField: "",
        cityList: [],
        cityTotal: 0,
        indexList: [],
        curIndex: "A",
        cinfo: {},
        targetIndex: "",
        searchRes: [],
        isPosition: 0,
        matchData: {}
    },
    onLoad: function(t) {
        var i = e.globalData.positionInfo, s = e.globalData.personPosition;
        i.status > -2 ? this.setData({
            isPosition: i.status,
            cinfo: i,
            isLoading: !0
        }) : this.setData({
            isLoading: !0
        });
        var o = wx.getSystemInfoSync();
        console.log(s.code), a.onMessage({
            theme: "getcity",
            data: {
                driveWidth: o.windowWidth,
                code: s.code
            }
        }, this.subscribeWorker);
    },
    onShow: function() {
        getApp().dot();
    },
    subscribeWorker: function(t) {
        switch (t.theme) {
          case "getcity":
            this.setData({
                cityList: t.data.list,
                total: t.data.total,
                matchData: t.data.matchData,
                isLoading: !1
            });
            break;

          case "findcity":
            this.setData({
                searchRes: t.data,
                isLoading: !1
            });
            break;

          case "findcitycode":
            this.setData({
                matchData: t.data,
                isLoading: !1
            });
        }
    },
    search: function(t) {
        var e = this.data.searchTip, i = t.detail.value;
        i.length > 0 ? (e = !1, a.onMessage({
            theme: "findcity",
            data: i
        }, this.subscribeWorker)) : e = !0, this.setData({
            searchField: i,
            searchTip: e
        });
    },
    scrollToByIndex: function(t) {
        var a = arguments.length > 1 && void 0 !== arguments[1] && arguments[1], e = t;
        a || (e = t.currentTarget.dataset.index), wx.showToast({
            title: e,
            icon: "none",
            duration: 800
        }), a ? this.setData({
            curIndex: e
        }) : this.setData({
            curIndex: e,
            targetIndex: e
        });
    },
    onscroll: function(t) {},
    searchTipFocus: function(t) {
        this.setData({
            isFocus: !0
        });
    },
    onblur: function() {
        this.data.searchField.length < 1 && this.setData({
            searchTip: !0
        });
    },
    bindChange: function(i) {
        var s = i.currentTarget.dataset, o = a.detail(s), n = t({
            status: 1
        }, o);
        e.globalData.positionInfo = Object.assign(e.globalData.positionInfo, n), wx.setStorage({
            key: "clear_search",
            data: !0,
            success: function(t) {
                wx.navigateBack();
            }
        });
    }
});