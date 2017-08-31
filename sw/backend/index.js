const GitHubApi = require("github");
const http = require('http');
const url = require('url');
const struct = require('./structs.js');

const LISTEN_PORT = 9615
const github = new GitHubApi({ debug: false });



function getContributors(callback) {
    github.repos.getStatsContributors({
        owner: "radare",
        repo: "radare2"
    }, function(err, res) {

        if (err) {
            console.log("Error getting contributors");
            console.log(err);
            callback(err);
            return;
        }

        const data = res.data;
        const meta = res.meta;

        struct.topContrib.buffer().fill(0);
        const proxy = struct.topContrib.fields;

        const contribs = data.length;
        for (let x=(contribs-1), y=0; x>(contribs-11); x--, y++) {
            //console.log("Contrib %d: %s (%d)", y, data[x].author.login, data[x].total);

            proxy.list[y].name = data[x].author.login.substring(0, 11);
            proxy.list[y].commits = data[x].total;
        }

        callback();
    });
}

function getStatus(callback)
{
    github.repos.getCombinedStatusForRef({
        owner: "radare",
        repo: "radare2",
        ref: "master"
    }, function(err, res) {

        if (err) {
            console.log("Error getting statuses");
            console.log(err);
            callback(err);
            return;
        }

        //console.log(JSON.stringify(res, null, 3));

        const data = res.data;
        const meta = res.meta;

        struct.masterStatus.buffer().fill(0);
        const proxy = struct.masterStatus.fields;

        proxy.global = data.state.substring(0, 15);

        for (let x=0; x<data.statuses.length; x++) {
            let status = data.statuses[x];
            if (status.context.indexOf('appveyor') !== -1)
                proxy.appveyor = status.state.substring(0, 15);
            else if (status.context.indexOf('travis-ci') !== -1)
                proxy.travis = status.state.substring(0, 15);
            else
                console.log('Unknown status...');
        }

        //console.log(struct.masterStatus.buffer());
        callback();
    });
}

function getIssuesAndPRs(callback) {

    github.issues.getForRepo({
        owner: "radare",
        repo: "radare2",
        per_page: 100
    }, function(err, res) {

        if (err) {
            console.log("Error getting issues");
            console.log(err);
            callback(err);
            return;
        }

        const data = res.data;
        const meta = res.meta;

        //console.log(JSON.stringify(res, null, 3));

        struct.pullRequests.buffer().fill(0);
        struct.issues.buffer().fill(0);

        const proxyPRs = struct.pullRequests.fields;
        const proxyIssues = struct.issues.fields;

        let issues = 0;
        let prs = 0;

        for (let x=0; x<data.length; x++) {
            let entry = data[x];

            if (entry.pull_request) {
                if (prs >= 10)
                    continue;

                proxyPRs.list[prs].title = entry.title.substring(0, 63);
                proxyPRs.list[prs].id = entry.id;
                prs++;
            } else {
                if (issues >= 10)
                    continue;

                proxyIssues.list[issues].title = entry.title.substring(0, 63);
                proxyIssues.list[issues].id = entry.id;
                issues++;
            }
        }

        //console.log(struct.pullRequests.buffer());
        //console.log(struct.issues.buffer());
        callback();
    });

}


function getCommits(callback) {

    github.repos.getCommits({
        owner: "radare",
        repo: "radare2",
        per_page: 10
    }, function(err, res) {

        if (err) {
            console.log("Error getting issues");
            console.log(err);
            callback(err);
            return;
        }

        const data = res.data;
        const meta = res.meta;

        //console.log(JSON.stringify(res, null, 3));

        struct.commits.buffer().fill(0);
        const proxy = struct.commits.fields;

        for (let x=0; x<10; x++)
            proxy.list[x].title = data[x].commit.message.substring(0, 63);
        
        callback();
    });

}


/* This sucks hard, but I do not have the time to make it pretty */
function updateStats(callback) {
    getContributors(function() {
        getStatus(function() {
            getIssuesAndPRs(function() {
                getCommits(function() {
                    console.log("Status updated");
                    if (callback)
                        callback();
                });              
            });
        });      
    });
}


function serverReply(response, data) {
    response.writeHead(200, {'Content-Length': data.length});
    response.end(data);
}

struct.init();

updateStats(function() {
    setInterval(updateStats, 5*60*1000); 
});


http.createServer(function (req, res) {

    let requrl = url.parse(req.url);
    //console.log(requrl);

    console.log('Got request for: ' + requrl.pathname);

    switch (requrl.pathname) {
        case '/r2k17/contribs':
            serverReply(res, struct.topContrib.buffer());
            break;
        case '/r2k17/status':
            serverReply(res, struct.masterStatus.buffer());
            break;
        case '/r2k17/prs':
            serverReply(res, struct.pullRequests.buffer());
            break;
        case '/r2k17/issues':
            serverReply(res, struct.issues.buffer());
            break;
        case '/r2k17/commits':
            serverReply(res, struct.commits.buffer());
            break;
        default:
            res.writeHead(404);
            res.end('Not found');
            break;
    }

}).listen(LISTEN_PORT);

console.log("head to: http://localhost:%d/", LISTEN_PORT);
