Component({
    behaviors: [],
    properties: {
        direction: String,
        mode: {
            type: String,
            value: "selector"
        },
        value: {
            type: Array,
            value: []
        },
        range: {
            type: Array,
            value: []
        },
        disabled: {
            type: Boolean,
            value: !1
        },
        title: {
            type: String,
            value: ""
        },
        lbtnStr: {
            type: String,
            value: "取消"
        },
        rbtnStr: {
            type: String,
            value: "确定"
        },
        lbtnColor: {
            type: String,
            value: "#666666;"
        },
        rbtnColor: {
            type: String,
            value: "#5192FF;"
        },
        indicatorStyle: {
            type: String,
            value: "#5192FF;"
        }
    },
    data: {
        _isMoving: !1,
        _visible: !1,
        _coverAnm: "",
        _pickerAnm: "",
        _value: [],
        _dateList: [],
        _dateDefault: [],
        _curData: []
    },
    lifetimes: {
        attached: function() {},
        moved: function() {},
        detached: function() {}
    },
    attached: function() {},
    ready: function() {},
    pageLifetimes: {
        show: function() {}
    },
    methods: {
        synDefaultValue: function(t, e) {
            var a = [], i = [];
            if (e.length < 1) for (var n = 0; n < t.length; n++) a.push(0); else e.forEach(function(e, n) {
                a.push(e), i[n] = null == i[n] ? [] : i[n], i[n].push(t[n][e]);
            });
            this.setData({
                _value: a,
                _curData: i
            });
        },
        getTs: function(t) {
            return t ? new Date(t.replace(/\-/g, "/")) : new Date();
        },
        getAllDays: function(t) {
            var e = this.getTs(t), a = e.getMonth() + 1;
            return e.setMonth(a), e.setDate(0), e.getDate();
        },
        makeDate: function() {
            for (var t = [], e = [], a = [], i = this.getTs(), n = i.getFullYear(), r = i.getMonth() + 1, s = i.getDay(), o = this.getAllDays(), u = 1900; u <= 2025; u++) t.push(u);
            for (var l = 1; l <= 12; l++) l = l <= 9 ? "0" + l : l, e.push(l);
            for (var h = 1; h <= o; h++) h = h <= 9 ? "0" + h : h, a.push(h);
            this.setData({
                _dateList: [ t, e, a ],
                _dateDefault: [ Number(n) - Number(1900), Number(r) - 1, Number(s) - 1 ],
                _value: [ Number(n) - Number(1900), Number(r) - 1, Number(s) - 1 ]
            });
        },
        show: function(t) {
            var e = this, a = t.currentTarget.dataset;
            "date" == a.mode ? this.makeDate() : (a.mode = "selector") && (this.synDefaultValue(a.range, a.dfvalue), 
            this.setData({
                _dateList: this.properties.range
            }));
            var i = wx.createAnimation({
                duration: 100
            }), n = wx.createAnimation({
                duration: 250
            });
            i.backgroundColor("rgba(0,0,0,0.4)").step(), n.translateY("-300px").step(), this.setData({
                _visible: !0
            }), setTimeout(function() {
                e.setData({
                    _coverAnm: i.export(),
                    _pickerAnm: n.export()
                });
            }, 100);
        },
        hide: function() {
            var t = this, e = wx.createAnimation({
                duration: 1
            }), a = wx.createAnimation({
                duration: 200
            });
            a.translateY("0").step(), this.setData({
                _pickerAnm: a.export()
            }), setTimeout(function() {
                e.backgroundColor("rgba(0,0,0,0)").step(), t.setData({
                    _coverAnm: e.export(),
                    _visible: !1
                });
            }, 200);
        },
        bindDateChange: function(t) {
            for (var e = this.data._dateList, a = t.detail.value, i = [], n = this.getAllDays(e[0][a[0]] + "-" + e[1][a[1]] + "-01"), r = 1; r <= n; r++) r = r <= 9 ? "0" + r : r, 
            i.push(r);
            e[2] = i, this.setData({
                _dateList: e,
                _value: a
            }), this.triggerEvent("onChange", {
                index: a,
                data: e[0][a[0]] + "-" + e[1][a[1]] + "-" + e[2][a[2]]
            });
        },
        bindChange: function(t) {
            for (var e = t.detail.value, a = [], i = 0; i < e.length; i++) a.push(e[i]);
            this.setData({
                _value: e
            }), this.triggerEvent("onChange", {
                index: e,
                data: a
            });
        },
        bindpickstart: function(t) {
            wx.showNavigationBarLoading(), this.setData({
                _isMoving: !0
            });
        },
        bindpickend: function(t) {
            wx.hideNavigationBarLoading(), this.setData({
                _isMoving: !1
            });
        },
        bindSelect: function() {
            if (this.data._isMoving) return !1;
            for (var t = [], e = 0; e < this.data._value.length; e++) t.push(this.data._dateList[e][this.data._value[e]]);
            this.triggerEvent("onSelect", {
                index: this.data._value,
                data: t
            }), this.hide();
        }
    }
});