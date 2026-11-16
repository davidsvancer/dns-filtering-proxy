#!/bin/bash

# config
DNS_PROXY="127.0.0.1"
DNS_PROXY_V6="::1"
DNS_PORT=53530
FILTER_FILE="domeny.txt"

# test domains
ALLOWED_DOMAIN="example.com"
ALLOWED_IPV6="google.com"
BLOCKED_DOMAIN="360yield.com"
BLOCKED_SUBDOMAIN="ads.360yield.com"
NON_A_DOMAIN="example.com MX"
INVALID_DOMAIN="@@@invalid@@@.com"
NONEXISTENT_DOMAIN="idontexist.example"

PASS=0
FAIL=0

test_query() {
    local resolver=$1
    local domain=$2
    local type=$3
    local expected=$4
    local note=$5

    echo -n "[Test] $resolver | $type $domain $note ... "

    response=$(dig @$resolver -p $DNS_PORT $domain $type +short 2>/dev/null)

    if [ "$expected" == "allow" ]; then
        if [ -n "$response" ]; then
            echo "PASSED"
            ((PASS++))
        else
            echo "FAILED"
            ((FAIL++))
        fi
    elif [ "$expected" == "block" ]; then
        if [ -z "$response" ]; then
            echo "PASSED"
            ((PASS++))
        else
            echo "FAILED"
            echo "Response: $response"
            ((FAIL++))
        fi
    elif [ "$expected" == "error" ]; then
        if [ -z "$response" ]; then
            echo "PASSED"
            ((PASS++))
        else
            echo "FAILED"
            ((FAIL++))
        fi
    fi
}

# test cases

echo "IPv4 Tests"
test_query $DNS_PROXY $ALLOWED_DOMAIN A allow "allowed A"
test_query $DNS_PROXY $BLOCKED_DOMAIN A block "blocked A"
test_query $DNS_PROXY $BLOCKED_SUBDOMAIN A block "blocked subdomain"
test_query $DNS_PROXY $ALLOWED_DOMAIN MX error "non-A type"
test_query $DNS_PROXY $ALLOWED_DOMAIN AAAA allow "IPv6 query over IPv4"
test_query $DNS_PROXY $NONEXISTENT_DOMAIN A error "nonexistent domain"
test_query $DNS_PROXY $INVALID_DOMAIN A error "invalid domain format"

echo
echo "IPv6 Tests"
test_query $DNS_PROXY_V6 $ALLOWED_IPV6 AAAA allow "IPv6 query allowed"
test_query $DNS_PROXY_V6 $BLOCKED_DOMAIN A block "blocked over IPv6 transport"
test_query $DNS_PROXY_V6 $ALLOWED_DOMAIN MX eror "MX via IPv6"
test_query $DNS_PROXY_V6 $ALLOWED_DOMAIN A allow "A query over IPv6 transport"
test_query $DNS_PROXY_V6 $BLOCKED_SUBDOMAIN A block "subdomain block IPv6"
test_query $DNS_PROXY_V6 $NONEXISTENT_DOMAIN AAAA error "nonexistent IPv6"

echo
echo "Edge cases"
test_query $DNS_PROXY "" A error "empty query"
test_query $DNS_PROXY "." A error "root domain"
test_query $DNS_PROXY "localhost" A error "localhost resolution"
test_query $DNS_PROXY "this.should.not.exist.invalid" A error "deep invalid"

test_query $DNS_PROXY "" AAAA error "empty query"
test_query $DNS_PROXY "." AAAA error "root domain"
test_query $DNS_PROXY "localhost" AAAA error "localhost resolution"
test_query $DNS_PROXY "this.should.not.exist.invalid" AAAA error "deep invalid"

# summary
echo
echo "========================================="
echo "Tests finished."
echo "Passed: $PASS"
echo "Failed: $FAIL"
echo "========================================="

# stress test
if command -v dnsperf >/dev/null 2>&1; then
    echo
    echo "--- Running dnsperf stress test ---"
    echo "$ALLOWED_DOMAIN A" > dnsperf_input.txt
    echo "$BLOCKED_DOMAIN A" >> dnsperf_input.txt
    echo "$ALLOWED_IPV6 AAAA" >> dnsperf_input.txt
    dnsperf -s $DNS_PROXY -p $DNS_PORT -d dnsperf_input.txt -l 10
else
    echo
    echo "dnsperf not found — skipping stress test."
fi