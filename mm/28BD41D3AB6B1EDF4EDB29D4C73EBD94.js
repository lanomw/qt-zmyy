var t = function() {
    function t(t, e) {
        for (var o = 0; o < e.length; o++) {
            var i = e[o];
            i.enumerable = i.enumerable || !1, i.configurable = !0, "value" in i && (i.writable = !0), 
            Object.defineProperty(t, i.key, i);
        }
    }
    return function(e, o, i) {
        return o && t(e.prototype, o), i && t(e, i), e;
    };
}();

var e = 310, o = "请求参数信息有误", i = 600, r = "系统错误", a = 1e3, n = 200, s = {
    location2query: function(t) {
        if ("string" == typeof t) return t;
        for (var e = "", o = 0; o < t.length; o++) {
            var i = t[o];
            e && (e += ";"), i.location && (e = e + i.location.lat + "," + i.location.lng), 
            i.latitude && i.longitude && (e = e + i.latitude + "," + i.longitude);
        }
        return e;
    },
    getWXLocation: function(t, e, o) {
        wx.getLocation({
            type: "gcj02",
            success: t,
            fail: e,
            complete: o
        });
    },
    getLocationParam: function(t) {
        if ("string" == typeof t) {
            var e = t.split(",");
            t = 2 === e.length ? {
                latitude: t.split(",")[0],
                longitude: t.split(",")[1]
            } : {};
        }
        return t;
    },
    polyfillParam: function(t) {
        t.success = t.success || function() {}, t.fail = t.fail || function() {}, t.complete = t.complete || function() {};
    },
    checkParamKeyEmpty: function(t, i) {
        if (!t[i]) {
            var r = this.buildErrorConfig(e, o + i + "参数格式有误");
            return t.fail(r), t.complete(r), !0;
        }
        return !1;
    },
    checkKeyword: function(t) {
        return !this.checkParamKeyEmpty(t, "keyword");
    },
    checkLocation: function(t) {
        var i = this.getLocationParam(t.location);
        if (!i || !i.latitude || !i.longitude) {
            var r = this.buildErrorConfig(e, o + " location参数格式有误");
            return t.fail(r), t.complete(r), !1;
        }
        return !0;
    },
    buildErrorConfig: function(t, e) {
        return {
            status: t,
            message: e
        };
    },
    buildWxRequestConfig: function(t, e) {
        var o = this;
        return e.header = {
            "content-type": "application/json"
        }, e.method = "GET", e.success = function(e) {
            var o = e.data;
            0 === o.status ? t.success(o) : t.fail(o);
        }, e.fail = function(e) {
            e.statusCode = a, t.fail(o.buildErrorConfig(a, result.errMsg));
        }, e.complete = function(e) {
            switch (+e.statusCode) {
              case a:
                t.complete(o.buildErrorConfig(a, e.errMsg));
                break;

              case n:
                var s = e.data;
                0 === s.status ? t.complete(s) : t.complete(o.buildErrorConfig(s.status, s.message));
                break;

              default:
                t.complete(o.buildErrorConfig(i, r));
            }
        }, e;
    },
    locationProcess: function(t, e, o, i) {
        var r = this;
        (o = o || function(e) {
            e.statusCode = a, t.fail(r.buildErrorConfig(a, e.errMsg));
        }, i = i || function(e) {
            e.statusCode == a && t.complete(r.buildErrorConfig(a, e.errMsg));
        }, t.location) ? r.checkLocation(t) && e(s.getLocationParam(t.location)) : r.getWXLocation(e, o, i);
    }
}, u = function() {
    function e(t) {
        if (function(t, e) {
            if (!(t instanceof e)) throw new TypeError("Cannot call a class as a function");
        }(this, e), !t.key) throw Error("key值不能为空");
        this.key = t.key;
    }
    return t(e, [ {
        key: "search",
        value: function(t) {
            if (t = t || {}, s.polyfillParam(t), s.checkKeyword(t)) {
                var e = {
                    keyword: t.keyword,
                    orderby: t.orderby || "_distance",
                    page_size: t.page_size || 10,
                    page_index: t.page_index || 1,
                    output: "json",
                    key: this.key
                };
                t.address_format && (e.address_format = t.address_format), t.filter && (e.filter = t.filter);
                var o = t.distance || "1000", i = t.auto_extend || 1;
                s.locationProcess(t, function(r) {
                    e.boundary = "nearby(" + r.latitude + "," + r.longitude + "," + o + "," + i + ")", 
                    wx.request(s.buildWxRequestConfig(t, {
                        url: "https://apis.map.qq.com/ws/place/v1/search",
                        data: e
                    }));
                });
            }
        }
    }, {
        key: "getSuggestion",
        value: function(t) {
            if (t = t || {}, s.polyfillParam(t), s.checkKeyword(t)) {
                var e = {
                    keyword: t.keyword,
                    region: t.region || "全国",
                    region_fix: t.region_fix || 0,
                    policy: t.policy || 0,
                    output: "json",
                    key: this.key
                };
                wx.request(s.buildWxRequestConfig(t, {
                    url: "https://apis.map.qq.com/ws/place/v1/suggestion",
                    data: e
                }));
            }
        }
    }, {
        key: "reverseGeocoder",
        value: function(t) {
            t = t || {}, s.polyfillParam(t);
            var e = {
                coord_type: t.coord_type || 5,
                get_poi: t.get_poi || 0,
                output: "json",
                key: this.key
            };
            t.poi_options && (e.poi_options = t.poi_options);
            s.locationProcess(t, function(o) {
                e.location = o.latitude + "," + o.longitude, wx.request(s.buildWxRequestConfig(t, {
                    url: "https://apis.map.qq.com/ws/geocoder/v1/",
                    data: e
                }));
            });
        }
    }, {
        key: "geocoder",
        value: function(t) {
            if (t = t || {}, s.polyfillParam(t), !s.checkParamKeyEmpty(t, "address")) {
                var e = {
                    address: t.address,
                    output: "json",
                    key: this.key
                };
                wx.request(s.buildWxRequestConfig(t, {
                    url: "https://apis.map.qq.com/ws/geocoder/v1/",
                    data: e
                }));
            }
        }
    }, {
        key: "getCityList",
        value: function(t) {
            t = t || {}, s.polyfillParam(t);
            var e = {
                output: "json",
                key: this.key
            };
            wx.request(s.buildWxRequestConfig(t, {
                url: "https://apis.map.qq.com/ws/district/v1/list",
                data: e
            }));
        }
    }, {
        key: "getDistrictByCityId",
        value: function(t) {
            if (t = t || {}, s.polyfillParam(t), !s.checkParamKeyEmpty(t, "id")) {
                var e = {
                    id: t.id || "",
                    output: "json",
                    key: this.key
                };
                wx.request(s.buildWxRequestConfig(t, {
                    url: "https://apis.map.qq.com/ws/district/v1/getchildren",
                    data: e
                }));
            }
        }
    }, {
        key: "calculateDistance",
        value: function(t) {
            if (t = t || {}, s.polyfillParam(t), !s.checkParamKeyEmpty(t, "to")) {
                var e = {
                    mode: t.mode || "walking",
                    to: s.location2query(t.to),
                    output: "json",
                    key: this.key
                };
                t.from && (t.location = t.from), s.locationProcess(t, function(o) {
                    e.from = o.latitude + "," + o.longitude, wx.request(s.buildWxRequestConfig(t, {
                        url: "https://apis.map.qq.com/ws/distance/v1/",
                        data: e
                    }));
                });
            }
        }
    } ]), e;
}();

module.exports = u;