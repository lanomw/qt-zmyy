var t = require("../../../3F1067E7AB6B1EDF59760FE01FCEBD94.js");

Component({
    options: {
        addGlobalClass: !0
    },
    properties: {
        tmVisable: {
            type: Boolean,
            value: !1
        },
        sticky: {
            type: Boolean,
            value: !1
        },
        pList: {
            type: Array,
            value: [ {
                id: 0,
                cname: "全部"
            } ]
        },
        cList: {
            type: Array,
            value: []
        },
        pActive: {
            type: Number,
            value: 0
        },
        cActive: {
            type: Number,
            value: 0
        }
    },
    pageLifetimes: {
        show: function() {}
    },
    data: {
        reloadVisible: !1
    },
    methods: {
        changeProjectActive: function(e) {
            var a = this, i = e.currentTarget.dataset.index, c = 0;
            if (i > 0) {
                var r = e.currentTarget.dataset.id, n = [];
                t.getVaccineCates(r, function(t) {
                    200 == t.data.status && (t.data.list.length > 0 && (c = t.data.list[0].id), n = t.data.list), 
                    a.triggerEvent("change", {
                        pActive: i,
                        cActive: 0,
                        product: c,
                        cList: n
                    });
                });
            } else this.triggerEvent("change", {
                pActive: i,
                cActive: 0,
                product: c,
                cList: []
            });
        },
        changeTypeActive: function(t) {
            this.triggerEvent("change", {
                cActive: t.currentTarget.dataset.index,
                product: t.currentTarget.dataset.product || 0
            });
        },
        chooseTi: function(t) {
            this.changeProjectActive(t);
        },
        handleTm: function(t) {
            this.triggerEvent("tmchange", t.currentTarget.dataset.modal);
        }
    }
});