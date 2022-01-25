var t = require("../../../EE492EF5AB6B1EDF882F46F2459EBD94.js");

Component({
    behaviors: [],
    properties: {
        data: Array
    },
    data: {
        mounthTitle: [ "日", "一", "二", "三", "四", "五", "六" ],
        curDays: [],
        curDay: 1,
        curTitle: "",
        dateAnimate: "",
        dateInit: 1,
        dateAnimate2: "",
        dateWidth: "",
        dateLen_1: "",
        dateLen_2: "",
        dateMoving: !1,
        dayAnimate: ""
    },
    lifetimes: {
        attached: function() {},
        moved: function() {},
        detached: function() {}
    },
    attached: function() {},
    ready: function() {},
    observers: {
        data: function(t) {
            this.renderDateDesc();
        }
    },
    pageLifetimes: {
        show: function() {
            var e = wx.getSystemInfoSync();
            this.setData({
                curTitle: t.dateFormate(),
                dateWidth: e.windowWidth,
                dateLen_1: 0,
                dateLen_2: -1 * e.windowWidth
            }), this.getCurrentMouth(this.data.curTitle);
        }
    },
    methods: {
        changeCalendar: function(e) {
            var a = this;
            if (!this.data.dateMoving) {
                var i = e.currentTarget.dataset.handle, n = this.data.curTitle.split("-"), r = this.data.dateLen_1, d = this.data.dateLen_2, s = t.getYear(), o = t.getMonth(), u = wx.createAnimation({
                    duration: 200,
                    timingFunction: "easing",
                    delay: 0
                }), h = wx.createAnimation({
                    duration: 200,
                    timingFunction: "easing",
                    delay: 0
                });
                if ("neg" == i) r -= this.data.dateWidth, d -= this.data.dateWidth, 12 == Number(n[1]) ? (n[1] = 1, 
                n[0] = Number(n[0]) + 1) : n[1] = Number(n[1]) + 1; else {
                    if (s == n[0] && o == n[1]) return wx.showToast({
                        title: "不能往前翻了",
                        icon: "none",
                        duration: 1500
                    }), !1;
                    r += this.data.dateWidth, d += this.data.dateWidth, 1 == Number(n[1]) ? (n[1] = 12, 
                    n[0] -= 1) : n[1] = Number(n[1]) - 1;
                }
                n[1] = n[1] <= 9 ? "0" + n[1] : n[1], n[2] = "01", this.triggerEvent("changeCalendar", {
                    curMouthArr: n.join("-")
                }), r > d && d <= -1 * this.data.dateWidth ? (r = -1 * this.data.dateWidth, d = 0, 
                u.translateX(0).step({
                    duration: 1
                }), h.translateX(this.data.dateWidth).step({
                    duration: 1
                }), this.setData({
                    dateAnimate: u.export(),
                    dateAnimate2: h.export(),
                    dateLen_1: 0,
                    dateLen_2: this.data.dateWidth,
                    dateMoving: !0
                }), setTimeout(function() {
                    u.translateX(r).step(), h.translateX(d).step(), a.setData({
                        dateAnimate2: h.export(),
                        dateAnimate: u.export(),
                        curTitle: n.join("-")
                    }), a.getCurrentMouth(n.join("-"));
                }, 100)) : (u.translateX(r).step(), h.translateX(d).step(), this.setData({
                    dateAnimate2: h.export(),
                    dateAnimate: u.export(),
                    curTitle: n.join("-")
                }), this.getCurrentMouth(n.join("-"))), setTimeout(function() {
                    u.translateX(0).step({
                        duration: 1
                    }), h.translateX(-1 * a.data.dateWidth).step({
                        duration: 1
                    }), a.setData({
                        dateLen_1: 0,
                        dateLen_2: -1 * a.data.dateWidth,
                        dateAnimate: u.export(),
                        dateAnimate2: h.export(),
                        dateMoving: !1
                    });
                }, 300);
            }
        },
        renderDateDesc: function() {
            var t = [], e = this.properties.data;
            if (!e[0]) return !1;
            var a = 0;
            this.data.curDays.forEach(function(i, n) {
                var r = [];
                i.forEach(function(t, i) {
                    1;
                    a < e.length && Number(e[0].date) <= t.date && t.month == Number(e[0].month) && t.date == e[a].date ? (t.disable = !1, 
                    t.desc = e[a].label, a++) : (t.disable = !0, t.desc = ""), r.push(t);
                }), t.push(r);
            }), this.setData({
                curDays: t
            });
        },
        setDate: function(t) {
            var e = t.currentTarget.dataset.date, a = this.data.curTitle.split("-"), i = wx.createAnimation({
                duration: 200
            });
            a[2] = e <= 9 ? "0" + e : e, i.scale(.8, .8).step({
                duration: 100
            }), i.scale(1, 1).step({
                duration: 100
            }), this.setData({
                curDay: e,
                curTitle: a.join("-"),
                dayAnimate: i.export()
            }), this.triggerEvent("changeTime", {
                curTime: a.join("-")
            });
        },
        getCurrentMouth: function(e) {
            var a = this, i = e.split("-"), n = i[2], r = t.getYear(e), d = t.getMonth(e);
            r == i[0] && d == i[1] && (n = t.getCurDate(e));
            var s = 1 == i[1] ? 12 : i[1] - 1, o = t.getDate(e);
            i[1] = s;
            var u = t.getDate(i.join("-")), h = e.split("-");
            h[2] = 1;
            for (var c = t.getDay(h.join("-")), l = [], m = 0; m < 5; m++) {
                for (var g = [], p = 0; p < 7; p++) {
                    var f = 1, D = !1, v = !1, T = d;
                    7 * m + p < c ? (f = u - (c - p - 1), T = d <= 1 ? 12 : d - 1, D = !0, v = !0) : 7 * m + p - c + 1 <= o ? f = 7 * m + p - c + 1 : (f = 7 * m + p - c + 1 - o, 
                    D = !0, v = !1);
                    var y = e.split("-");
                    y[2] = f;
                    var A = t.getTs(y.join("-")), x = t.getTs() - 864e5;
                    g.push({
                        disable: D || A < x,
                        date: f,
                        month: T,
                        isOld: A < x,
                        readonly: v || A < x,
                        desc: ""
                    });
                }
                l.push(g);
            }
            a.setData({
                curDays: l,
                curDay: n
            }, function() {
                a.renderDateDesc();
            });
        }
    }
});