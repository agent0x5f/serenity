/*
 * Copyright (c) 2021-2022, Linus Groh <linusg@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Optional.h>
#include <LibJS/Heap/Handle.h>
#include <LibJS/Runtime/Completion.h>
#include <LibJS/Runtime/Object.h>
#include <LibJS/Runtime/Temporal/PlainDate.h>
#include <LibJS/Runtime/Temporal/PlainDateTime.h>
#include <LibJS/Runtime/Temporal/ZonedDateTime.h>
#include <LibJS/Runtime/VM.h>

namespace JS::Temporal {

class Duration final : public Object {
    JS_OBJECT(Duration, Object);

public:
    Duration(double years, double months, double weeks, double days, double hours, double minutes, double seconds, double milliseconds, double microseconds, double nanoseconds, Object& prototype);
    virtual ~Duration() override = default;

    [[nodiscard]] double years() const { return m_years; }
    [[nodiscard]] double months() const { return m_months; }
    [[nodiscard]] double weeks() const { return m_weeks; }
    [[nodiscard]] double days() const { return m_days; }
    [[nodiscard]] double hours() const { return m_hours; }
    [[nodiscard]] double minutes() const { return m_minutes; }
    [[nodiscard]] double seconds() const { return m_seconds; }
    [[nodiscard]] double milliseconds() const { return m_milliseconds; }
    [[nodiscard]] double microseconds() const { return m_microseconds; }
    [[nodiscard]] double nanoseconds() const { return m_nanoseconds; }

private:
    // 7.4 Properties of Temporal.Duration Instances, https://tc39.es/proposal-temporal/#sec-properties-of-temporal-duration-instances
    double m_years;        // [[Years]]
    double m_months;       // [[Months]]
    double m_weeks;        // [[Weeks]]
    double m_days;         // [[Days]]
    double m_hours;        // [[Hours]]
    double m_minutes;      // [[Minutes]]
    double m_seconds;      // [[Seconds]]
    double m_milliseconds; // [[Milliseconds]]
    double m_microseconds; // [[Microseconds]]
    double m_nanoseconds;  // [[Nanoseconds]]
};

// 7.5.1 Duration Records, https://tc39.es/proposal-temporal/#sec-temporal-duration-records
struct DurationRecord {
    double years;
    double months;
    double weeks;
    double days;
    double hours;
    double minutes;
    double seconds;
    double milliseconds;
    double microseconds;
    double nanoseconds;
};

// 7.5.2 Date Duration Records, https://tc39.es/proposal-temporal/#sec-temporal-date-duration-records
struct DateDurationRecord {
    double years;
    double months;
    double weeks;
    double days;
};

// 7.5.3 Time Duration Records, https://tc39.es/proposal-temporal/#sec-temporal-time-duration-records
struct TimeDurationRecord {
    double days;
    double hours;
    double minutes;
    double seconds;
    double milliseconds;
    double microseconds;
    double nanoseconds;
};

// 7.5.4 Partial Duration Records, https://tc39.es/proposal-temporal/#sec-temporal-partial-duration-records
struct PartialDurationRecord {
    Optional<double> years;
    Optional<double> months;
    Optional<double> weeks;
    Optional<double> days;
    Optional<double> hours;
    Optional<double> minutes;
    Optional<double> seconds;
    Optional<double> milliseconds;
    Optional<double> microseconds;
    Optional<double> nanoseconds;
};

// Used by MoveRelativeDate to temporarily hold values
struct MoveRelativeDateResult {
    Handle<PlainDate> relative_to;
    double days;
};

// Used by RoundDuration to temporarily hold values
struct RoundedDuration {
    DurationRecord duration_record;
    double remainder;
};

// Table 7: Properties of a TemporalDurationLike, https://tc39.es/proposal-temporal/#table-temporal-temporaldurationlike-properties

template<typename StructT, typename ValueT>
struct TemporalDurationLikeProperty {
    ValueT StructT::*field { nullptr };
    PropertyKey property;
};

template<typename StructT, typename ValueT>
auto temporal_duration_like_properties = [](VM& vm) {
    using PropertyT = TemporalDurationLikeProperty<StructT, ValueT>;
    return AK::Array<PropertyT, 10> {
        PropertyT { &StructT::days, vm.names.days },
        PropertyT { &StructT::hours, vm.names.hours },
        PropertyT { &StructT::microseconds, vm.names.microseconds },
        PropertyT { &StructT::milliseconds, vm.names.milliseconds },
        PropertyT { &StructT::minutes, vm.names.minutes },
        PropertyT { &StructT::months, vm.names.months },
        PropertyT { &StructT::nanoseconds, vm.names.nanoseconds },
        PropertyT { &StructT::seconds, vm.names.seconds },
        PropertyT { &StructT::weeks, vm.names.weeks },
        PropertyT { &StructT::years, vm.names.years },
    };
};

DurationRecord create_duration_record(double years, double months, double weeks, double days, double hours, double minutes, double seconds, double milliseconds, double microseconds, double nanoseconds);
ThrowCompletionOr<DurationRecord> create_duration_record(GlobalObject&, double years, double months, double weeks, double days, double hours, double minutes, double seconds, double milliseconds, double microseconds, double nanoseconds);
DateDurationRecord create_date_duration_record(double years, double months, double weeks, double days);
ThrowCompletionOr<DateDurationRecord> create_date_duration_record(GlobalObject&, double years, double months, double weeks, double days);
TimeDurationRecord create_time_duration_record(double days, double hours, double minutes, double seconds, double milliseconds, double microseconds, double nanoseconds);
ThrowCompletionOr<TimeDurationRecord> create_time_duration_record(GlobalObject&, double days, double hours, double minutes, double seconds, double milliseconds, double microseconds, double nanoseconds);
ThrowCompletionOr<Duration*> to_temporal_duration(GlobalObject&, Value item);
ThrowCompletionOr<DurationRecord> to_temporal_duration_record(GlobalObject&, Value temporal_duration_like);
i8 duration_sign(double years, double months, double weeks, double days, double hours, double minutes, double seconds, double milliseconds, double microseconds, double nanoseconds);
bool is_valid_duration(double years, double months, double weeks, double days, double hours, double minutes, double seconds, double milliseconds, double microseconds, double nanoseconds);
StringView default_temporal_largest_unit(double years, double months, double weeks, double days, double hours, double minutes, double seconds, double milliseconds, double microseconds);
ThrowCompletionOr<PartialDurationRecord> to_partial_duration(GlobalObject&, Value temporal_duration_like);
ThrowCompletionOr<Duration*> create_temporal_duration(GlobalObject&, double years, double months, double weeks, double days, double hours, double minutes, double seconds, double milliseconds, double microseconds, double nanoseconds, FunctionObject const* new_target = nullptr);
Duration* create_negated_temporal_duration(GlobalObject& global_object, Duration const& duration);
ThrowCompletionOr<double> calculate_offset_shift(GlobalObject&, Value relative_to_value, double years, double months, double weeks, double days, double hours, double minutes, double seconds, double milliseconds, double microseconds, double nanoseconds);
BigInt* total_duration_nanoseconds(GlobalObject&, double days, double hours, double minutes, double seconds, double milliseconds, double microseconds, BigInt const& nanoseconds, double offset_shift);
ThrowCompletionOr<TimeDurationRecord> balance_duration(GlobalObject&, double days, double hours, double minutes, double seconds, double milliseconds, double microseconds, BigInt const& nanoseconds, String const& largest_unit, Object* relative_to = nullptr);
ThrowCompletionOr<DateDurationRecord> unbalance_duration_relative(GlobalObject&, double years, double months, double weeks, double days, String const& largest_unit, Value relative_to);
ThrowCompletionOr<DateDurationRecord> balance_duration_relative(GlobalObject&, double years, double months, double weeks, double days, String const& largest_unit, Value relative_to);
ThrowCompletionOr<DurationRecord> add_duration(GlobalObject&, double years1, double months1, double weeks1, double days1, double hours1, double minutes1, double seconds1, double milliseconds1, double microseconds1, double nanoseconds1, double years2, double months2, double weeks2, double days2, double hours2, double minutes2, double seconds2, double milliseconds2, double microseconds2, double nanoseconds2, Value relative_to_value);
ThrowCompletionOr<MoveRelativeDateResult> move_relative_date(GlobalObject&, Object& calendar, PlainDate& relative_to, Duration& duration);
ThrowCompletionOr<ZonedDateTime*> move_relative_zoned_date_time(GlobalObject&, ZonedDateTime&, double years, double months, double weeks, double days);
ThrowCompletionOr<RoundedDuration> round_duration(GlobalObject&, double years, double months, double weeks, double days, double hours, double minutes, double seconds, double milliseconds, double microseconds, double nanoseconds, u32 increment, StringView unit, StringView rounding_mode, Object* relative_to_object = nullptr);
ThrowCompletionOr<DurationRecord> adjust_rounded_duration_days(GlobalObject& global_object, double years, double months, double weeks, double days, double hours, double minutes, double seconds, double milliseconds, double microseconds, double nanoseconds, u32 increment, StringView unit, StringView rounding_mode, Object* relative_to_object = nullptr);
ThrowCompletionOr<DurationRecord> to_limited_temporal_duration(GlobalObject&, Value temporal_duration_like, Vector<StringView> const& disallowed_fields);
String temporal_duration_to_string(double years, double months, double weeks, double days, double hours, double minutes, double seconds, double milliseconds, double microseconds, double nanoseconds, Variant<StringView, u8> const& precision);

// 7.5.22 DaysUntil ( earlier, later ), https://tc39.es/proposal-temporal/#sec-temporal-daysuntil
template<typename EarlierObjectType, typename LaterObjectType>
double days_until(GlobalObject& global_object, EarlierObjectType& earlier, LaterObjectType& later)
{
    // 1. Let difference be ! DifferenceISODate(earlier.[[ISOYear]], earlier.[[ISOMonth]], earlier.[[ISODay]], later.[[ISOYear]], later.[[ISOMonth]], later.[[ISODay]], "day").
    auto difference = difference_iso_date(global_object, earlier.iso_year(), earlier.iso_month(), earlier.iso_day(), later.iso_year(), later.iso_month(), later.iso_day(), "day"sv);

    // 2. Return difference.[[Days]].
    return difference.days;
}

}
