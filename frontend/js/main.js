$(document).ready(function() {
    'use strict'; // Prevents certain (unsafe) actions from being taken and throws more exceptions.
    const BTN_CLEAR = '#clearMailbox';
    const H1_MAILS = '#mails';
    const URL = 'http://192.168.1.9/arduino/digital/';

    $.getJSON(URL + 0, function(data, status){
        toggle(data.mails);
    });

    $(BTN_CLEAR).click(function() {
        $.getJSON(URL + 1, function(data, status){
            toggle(data.mails);
        });
    });

    function toggle(mails) {
        $(H1_MAILS).text('Snail mails in your mailbox: ' + mails);
        if(mails > 0) {
            $(BTN_CLEAR).removeClass('hidden');
        }
        else {
            $(BTN_CLEAR).addClass('hidden');
        }
    }
});
