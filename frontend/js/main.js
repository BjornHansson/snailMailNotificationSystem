$(document).ready(function() {
    'use strict'; // Prevents certain (unsafe) actions from being taken and throws more exceptions.
    const BTN_CLEAR = '#clearMailbox';
    const H1_MAILS = '#mails';
    const URL = 'http://192.168.1.9/arduino/digital/1';

    $.getJSON(URL, function(data, status){
        toggle(data.mails);
    });

    $(BTN_CLEAR).click(function() {
        $.post(URL, JSON.stringify({
            'clear': true
        }),
        function(data, status) {
            toggle(data.mails);
        });
    });

    function toggle(mails) {
        $(H1_MAILS).text(mails + ' snail mails in your mailbox');
        if(mails > 0) {
            $(BTN_CLEAR).removeClass('hidden');
        }
        else {
            $(BTN_CLEAR).addClass('hidden');
        }
    }
});
