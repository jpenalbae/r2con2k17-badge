'use strict';

const Struct = require('struct');

/* TOP CONTRIBUTORS */
const topContrib = Struct()
    .array('list', 10, Struct()
        .chars('name', 12)
        .word32Ule('commits')
    );


/* MASTER STATUS */
const masterStatus  = Struct()
    .chars('global', 16)
    .chars('travis', 16)
    .chars('appveyor', 16)


/* PULL REQUESTS */
const pullRequests = Struct()
    .array('list', 10, Struct()
        .chars('title', 64)
        .word32Ule('id')
    );


/* ISSUES */
const issues = Struct()
    .array('list', 10, Struct()
        .chars('title', 64)
        .word32Ule('id')
    );


/* COMMITS */
const commits = Struct()
    .array('list', 10, Struct()
        .chars('title', 64)
    );



/* Init all the structures */
function init()
{
    topContrib.allocate();
    masterStatus.allocate();
    pullRequests.allocate();
    issues.allocate();
    commits.allocate();

}


module.exports.topContrib = topContrib;
module.exports.masterStatus = masterStatus;
module.exports.pullRequests = pullRequests;
module.exports.issues = issues;
module.exports.commits = commits;

module.exports.init = init;