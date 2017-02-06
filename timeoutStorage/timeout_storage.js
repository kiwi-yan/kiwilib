/**
 * A storage tools based sessionStorage with timeout.
 * author: kiwiyan
 * date: 2017/02
 */

;timeoutStorage = (function() {
    if (typeof window.sessionStorage == 'undefined') {
        console.warn("Your browser does not support session storage.");
        return undefined;
    }
    return {
        setItem: function(key, val, timeout) {
            if (typeof key != 'string') {
                console.warn("The key should be string.");
            }
            var expireTime = 0;
            var timeout_type = typeof timeout;
            switch (timeout_type) {
            case 'undefined': break;
            case 'number': 
                timeout_ = parseInt(timeout);
                if (timeout_ < 0) {
                    console.error('The argument timeout should be positive.');
                    return;
                }
                if (timeout_ > 0) {
                    expireTime = (new Date()).getTime() + timeout_ * 1000;
                }
                break;
            default: console.error('The argument timeout should be a positive integer, not ' + timeout_type + '.');return;
            }
            res = JSON.stringify({val: val, expires: expireTime});
            if ('__ts_' + key)
            sessionStorage.setItem('__ts_' + key, res);
        },
        
        getItem: function(key) {
            res = sessionStorage.getItem('__ts_' + key);
            if (res != null) {
                obj = JSON.parse(res);
                if (obj.expires == 0 || obj.expires >= (new Date()).getTime()) {
                    return obj.val;
                }    
            }
            return null;
        },
        
        removeItem: function(key) {
            sessionStorage.removeItem('__ts_' + key);
        },
        
        clear: function() {
            delete_list = [];
            for (var i = 0; i < sessionStorage.length; i++) {
                key = sessionStorage.key(i);
                if (key.substr(0, 5) == '__ts_') {
                    delete_list.push(key);
                }
            }
            for (var i = 0; i < delete_list.length; i++) {
                sessionStorage.removeItem(delete_list[i]);
            }
        }
    };
})();