/*
BSD 3-Clause License

Copyright (c) 2017, Alibaba Cloud
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "timestamp.hpp"
#include "tablestore/util/prettyprint.hpp"
#include <boost/chrono/chrono.hpp>
#include <boost/thread/thread.hpp>
#include <boost/system/error_code.hpp>
#include <deque>

using namespace std;
using namespace boost::chrono;

namespace aliyun {
namespace tablestore {
namespace util {

void sleepFor(const Duration& d)
{
    boost::this_thread::sleep_for(microseconds(d.toUsec()));
}

void sleepUntil(const MonotonicTime& target)
{
    Duration d = target - MonotonicTime::now();
    if (d > Duration::fromUsec(0)) {
        sleepFor(d);
    }
}

namespace {

void fill(string& out, int64_t val, int64_t width)
{
    deque<char> digits;
    int64_t i = 0;
    for (; val > 0; ++i, val /= 10) {
        digits.push_back('0' + (val % 10));
    }
    for (; i < width; ++i) {
        digits.push_back('0');
    }

    for (; !digits.empty(); digits.pop_back()) {
        out.push_back(digits.back());
    }
}

} // namespace

void Duration::prettyPrint(string& out) const
{
    pp::prettyPrint(out, mValue / kUsecPerHour);
    out.push_back(':');
    fill(out, (mValue % kUsecPerHour) / kUsecPerMin, 2);
    out.push_back(':');
    fill(out, (mValue % kUsecPerMin) / kUsecPerSec, 2);
    out.push_back('.');
    fill(out, mValue % kUsecPerSec, 6);
}

void MonotonicTime::prettyPrint(string& out) const
{
    pp::prettyPrint(out, mValue / kUsecPerHour);
    out.push_back(':');
    fill(out, (mValue % kUsecPerHour) / kUsecPerMin, 2);
    out.push_back(':');
    fill(out, (mValue % kUsecPerMin) / kUsecPerSec, 2);
    out.push_back('.');
    fill(out, mValue % kUsecPerSec, 6);
}

MonotonicTime MonotonicTime::now()
{
    boost::system::error_code err;
    steady_clock::time_point sc = steady_clock::now(err);
    OTS_ASSERT(!err)(err.message());
    steady_clock::duration d = sc.time_since_epoch();
    microseconds usec = duration_cast<microseconds>(d);
    return MonotonicTime(usec.count());
}


namespace {

struct TimeComponent
{
    TimeComponent()
        : mYear(0), mMonth(0), mDay(0), mHour(0),
        mMinute(0), mSec(0), mUsec(0)
    {}

    int mYear;
    int mMonth;
    int mDay;
    int mHour;
    int mMinute;
    int mSec;
    int mUsec;
};

TimeComponent decompose(const UtcTime& tm)
{
    TimeComponent result;
    int64_t t = tm.toUsec();
    int64_t q = t / kUsecPerSec;
    result.mUsec = t % kUsecPerSec;
    time_t x = q;
    struct tm ti;
#ifdef _MSC_VER
    auto r = gmtime_s(&ti, &x);
    OTS_ASSERT(r == 0).what("time out of range");
#else
    struct tm* r = gmtime_r(&x, &ti);
    OTS_ASSERT(r != NULL).what("time out of range");
#endif
    result.mYear = ti.tm_year + 1900;
    result.mMonth = ti.tm_mon + 1;
    result.mDay = ti.tm_mday;
    result.mHour = ti.tm_hour;
    result.mMinute = ti.tm_min;
    result.mSec = ti.tm_sec;
    return result;
}

} // namespace

void UtcTime::toIso8601(string& out) const
{
    const TimeComponent& tc = decompose(*this);
    fill(out, tc.mYear, 4);
    out.push_back('-');
    fill(out, tc.mMonth, 2);
    out.push_back('-');
    fill(out, tc.mDay, 2);
    out.push_back('T');
    fill(out, tc.mHour, 2);
    out.push_back(':');
    fill(out, tc.mMinute, 2);
    out.push_back(':');
    fill(out, tc.mSec, 2);
    out.push_back('.');
    fill(out, tc.mUsec, 6);
    out.push_back('Z');
}

string UtcTime::toIso8601() const
{
    string res;
    res.reserve(sizeof("1970-01-01T00:00:00.000000Z"));
    toIso8601(res);
    return res;
}

void UtcTime::prettyPrint(string& out) const
{
    out.push_back('"');
    toIso8601(out);
    out.push_back('"');
}

UtcTime UtcTime::now()
{
    boost::system::error_code err;
    system_clock::time_point sc = system_clock::now(err);
    OTS_ASSERT(!err)(err.message());
    system_clock::duration d = sc.time_since_epoch();
    microseconds usec = duration_cast<microseconds>(d);
    return UtcTime(usec.count());
}

} // namespace util
} // namespace tablestore
} // namespace aliyun
