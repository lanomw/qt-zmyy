new Date();

var t = {
    dn: function(t) {
        return t ? new Date(t.replace(/-/g, "/")) : new Date();
    },
    dateFormate: function(t, e) {
        var n = this.dn(t), r = [ n.getFullYear(), this.getMonth() < 10 ? "0" + this.getMonth() : this.getMonth(), n.getDate() < 10 ? "0" + n.getDate() : n.getDate() ];
        return Boolean(e) || (e = "-"), r.join(e);
    },
    getYear: function(t) {
        return this.dn(t).getFullYear();
    },
    getMonth: function(t) {
        return this.dn(t).getMonth() + 1;
    },
    getCurDate: function(t) {
        return this.dn(t).getDate();
    },
    getDate: function(t) {
        var e = this.dn(t), n = this.getMonth(t);
        return e.setMonth(n), e.setDate(0), e.getDate();
    },
    getDay: function(t) {
        var e = this.dn(t), n = this.dateFormate(t);
        return e.setDate(n.split("-")[2]), e.getDay();
    },
    getTs: function(t) {
        return Date.parse(this.dn(t));
    }
};

module.exports = t;