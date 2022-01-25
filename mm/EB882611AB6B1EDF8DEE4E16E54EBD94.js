require("@babel/runtime/helpers/Arrayincludes.js");

var e = require("FD41D4A3AB6B1EDF9B27BCA4A45EBD94.js"), t = {}, c = {}, a = [ "重庆市", "上海市", "北京市", "天津市", "台湾省", "香港特别行政区", "澳门特别行政区" ];

function n(a) {
    var n = 0, i = Math.ceil(56 / 414 * a.driveWidth), r = {};
    return Object.keys(e).forEach(function(o) {
        var d = e[o], f = !1;
        n += i, Object.keys(d).forEach(function(e) {
            1 * a.code == 1 * d[e].adcode && (r = d[e]), "district" != d[e].level && (t[o] = t[o] ? t[o] : {}, 
            t[o][e] = t[o][e] ? t[o][e] : {}, t[o][e] = d[e], n += i, f = !0);
        }), f && (c[o] = n);
    }), {
        list: t,
        total: c,
        matchData: r
    };
}

function i(t) {
    var c = [];
    return Object.keys(e).forEach(function(a) {
        var n = e[a];
        Object.keys(n).forEach(function(e) {
            -1 !== n[e].name.indexOf(t) && c.push(n[e]);
        });
    }), c;
}

function r(t) {
    var c = {};
    return Object.keys(e).forEach(function(a) {
        var n = e[a];
        Object.keys(n).forEach(function(e) {
            1 * n[e].adcode == 1 * t && (c = n[e]);
        });
    }), c;
}

function o(t) {
    var c = t.code, a = t.pCode, n = {};
    try {
        Object.keys(e).forEach(function(t) {
            var a = e[t];
            Object.keys(a).forEach(function(e) {
                if (e == c) throw n = a[e], "找到了";
            });
        });
    } catch (e) {}
    if (!n.center) try {
        Object.keys(e).forEach(function(t) {
            var i = e[t];
            Object.keys(i).forEach(function(e) {
                if (e == a) throw n = i[e], c = a, "找到了";
            });
        });
    } catch (e) {}
    var i = n.center.split(",");
    return !!n.center && {
        lat: 1 * i[1],
        lng: 1 * i[0],
        region: d(n),
        name: n.name,
        code: c,
        detail: n
    };
}

function d(e) {
    var t = arguments.length > 1 && void 0 !== arguments[1] ? arguments[1] : [ "", "", "" ];
    return "district" == e.level ? t[2] = e.name : "city" == e.level ? t[1] = e.name : "province" == e.level && (t[0] = e.name, 
    a.includes(e.name) && (t[1] = e.name)), e.parent.name && d(e.parent, t), t;
}

module.exports = {
    get: n,
    find: i,
    detail: o,
    findcode: r,
    onMessage: function(e, t) {
        switch (e.theme) {
          case "getcity":
            t({
                theme: "getcity",
                data: n(e.data)
            });
            break;

          case "findcity":
            t({
                theme: "findcity",
                data: i(e.data)
            });
            break;

          case "findcitycode":
            t({
                theme: "findcitycode",
                data: r(e.data)
            });
            break;

          case "detailcity":
            t({
                theme: "detailcity",
                data: o(e.data)
            });
        }
    }
};