#undef TRACE_SYSTEM
#define TRACE_SYSTEM power

#if !defined(_TRACE_POWER_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_POWER_H

#include <linux/ktime.h>
#include <linux/pm_qos.h>
#include <linux/tracepoint.h>

DECLARE_EVENT_CLASS(cpu,

	TP_PROTO(unsigned int state, unsigned int cpu_id),

	TP_ARGS(state, cpu_id),

	TP_STRUCT__entry(
		__field(	u32,		state		)
		__field(	u32,		cpu_id		)
	),

	TP_fast_assign(
		__entry->state = state;
		__entry->cpu_id = cpu_id;
	),

	TP_printk("state=%lu cpu_id=%lu", (unsigned long)__entry->state,
		  (unsigned long)__entry->cpu_id)
);

DEFINE_EVENT(cpu, cpu_idle,

	TP_PROTO(unsigned int state, unsigned int cpu_id),

	TP_ARGS(state, cpu_id)
);

/* This file can get included multiple times, TRACE_HEADER_MULTI_READ at top */
#ifndef _PWR_EVENT_AVOID_DOUBLE_DEFINING
#define _PWR_EVENT_AVOID_DOUBLE_DEFINING

#define PWR_EVENT_EXIT -1
#endif

DEFINE_EVENT(cpu, cpu_frequency,

	TP_PROTO(unsigned int frequency, unsigned int cpu_id),

	TP_ARGS(frequency, cpu_id)
);

TRACE_EVENT(cpu_frequency_limits,

	TP_PROTO(unsigned int max_freq, unsigned int min_freq,
		unsigned int cpu_id),

	TP_ARGS(max_freq, min_freq, cpu_id),

	TP_STRUCT__entry(
		__field(	u32,		min_freq	)
		__field(	u32,		max_freq	)
		__field(	u32,		cpu_id		)
	),

	TP_fast_assign(
		__entry->min_freq = min_freq;
		__entry->max_freq = max_freq;
		__entry->cpu_id = cpu_id;
	),

	TP_printk("min=%lu max=%lu cpu_id=%lu",
		  (unsigned long)__entry->min_freq,
		  (unsigned long)__entry->max_freq,
		  (unsigned long)__entry->cpu_id)
);

TRACE_EVENT(cpu_frequency_switch_start,

	TP_PROTO(unsigned int start_freq, unsigned int end_freq,
		 unsigned int cpu_id),

	TP_ARGS(start_freq, end_freq, cpu_id),

	TP_STRUCT__entry(
		__field(	u32,		start_freq	)
		__field(	u32,		end_freq	)
		__field(	u32,		cpu_id		)
	),

	TP_fast_assign(
		__entry->start_freq = start_freq;
		__entry->end_freq = end_freq;
		__entry->cpu_id = cpu_id;
	),

	TP_printk("start=%lu end=%lu cpu_id=%lu",
		  (unsigned long)__entry->start_freq,
		  (unsigned long)__entry->end_freq,
		  (unsigned long)__entry->cpu_id)
);

TRACE_EVENT(cpu_frequency_switch_end,

	TP_PROTO(unsigned int cpu_id),

	TP_ARGS(cpu_id),

	TP_STRUCT__entry(
		__field(	u32,		cpu_id		)
	),

	TP_fast_assign(
		__entry->cpu_id = cpu_id;
	),

	TP_printk("cpu_id=%lu", (unsigned long)__entry->cpu_id)
);

DECLARE_EVENT_CLASS(set,
	TP_PROTO(u32 cpu_id, unsigned long currfreq,
			unsigned long load),
	TP_ARGS(cpu_id, currfreq, load),

	TP_STRUCT__entry(
	    __field(u32, cpu_id)
	    __field(unsigned long, currfreq)
	    __field(unsigned long, load)
	),

	TP_fast_assign(
	    __entry->cpu_id = (u32) cpu_id;
	    __entry->currfreq = currfreq;
	    __entry->load = load;
	),

	TP_printk("cpu=%u currfreq=%lu load=%lu",
	      __entry->cpu_id, __entry->currfreq,
	      __entry->load)
);

DEFINE_EVENT(set, cpufreq_sampling_event,
	TP_PROTO(u32 cpu_id, unsigned long currfreq,
		unsigned long load),
	TP_ARGS(cpu_id, currfreq, load)
);

DEFINE_EVENT(set, cpufreq_freq_synced,
	TP_PROTO(u32 cpu_id, unsigned long currfreq,
		unsigned long load),
	TP_ARGS(cpu_id, currfreq, load)
);

TRACE_EVENT(machine_suspend,

	TP_PROTO(unsigned int state),

	TP_ARGS(state),

	TP_STRUCT__entry(
		__field(	u32,		state		)
	),

	TP_fast_assign(
		__entry->state = state;
	),

	TP_printk("state=%lu", (unsigned long)__entry->state)
);

TRACE_EVENT(device_pm_report_time,

	TP_PROTO(struct device *dev, const char *pm_ops, s64 ops_time,
		 char *pm_event_str, int error),

	TP_ARGS(dev, pm_ops, ops_time, pm_event_str, error),

	TP_STRUCT__entry(
		__string(device, dev_name(dev))
		__string(driver, dev_driver_string(dev))
		__string(parent, dev->parent ? dev_name(dev->parent) : "none")
		__string(pm_ops, pm_ops ? pm_ops : "none ")
		__string(pm_event_str, pm_event_str)
		__field(s64, ops_time)
		__field(int, error)
	),

	TP_fast_assign(
		const char *tmp = dev->parent ? dev_name(dev->parent) : "none";
		const char *tmp_i = pm_ops ? pm_ops : "none ";

		__assign_str(device, dev_name(dev));
		__assign_str(driver, dev_driver_string(dev));
		__assign_str(parent, tmp);
		__assign_str(pm_ops, tmp_i);
		__assign_str(pm_event_str, pm_event_str);
		__entry->ops_time = ops_time;
		__entry->error = error;
	),

	/* ops_str has an extra space at the end */
	TP_printk("%s %s parent=%s state=%s ops=%snsecs=%lld err=%d",
		__get_str(driver), __get_str(device), __get_str(parent),
		__get_str(pm_event_str), __get_str(pm_ops),
		__entry->ops_time, __entry->error)
);

DECLARE_EVENT_CLASS(wakeup_source,

	TP_PROTO(const char *name, unsigned int state),

	TP_ARGS(name, state),

	TP_STRUCT__entry(
		__string(       name,           name            )
		__field(        u64,            state           )
	),

	TP_fast_assign(
		__assign_str(name, name);
		__entry->state = state;
	),

	TP_printk("%s state=0x%lx", __get_str(name),
		(unsigned long)__entry->state)
);

DEFINE_EVENT(wakeup_source, wakeup_source_activate,

	TP_PROTO(const char *name, unsigned int state),

	TP_ARGS(name, state)
);

DEFINE_EVENT(wakeup_source, wakeup_source_deactivate,

	TP_PROTO(const char *name, unsigned int state),

	TP_ARGS(name, state)
);

/*
 * The clock events are used for clock enable/disable and for
 *  clock rate change
 */
DECLARE_EVENT_CLASS(clock,

	TP_PROTO(const char *name, unsigned int state, unsigned int cpu_id),

	TP_ARGS(name, state, cpu_id),

	TP_STRUCT__entry(
		__string(       name,           name            )
		__field(        u64,            state           )
		__field(        u64,            cpu_id          )
	),

	TP_fast_assign(
		__assign_str(name, name);
		__entry->state = state;
		__entry->cpu_id = cpu_id;
	),

	TP_printk("%s state=%lu cpu_id=%lu", __get_str(name),
		(unsigned long)__entry->state, (unsigned long)__entry->cpu_id)
);

DEFINE_EVENT(clock, clock_enable,

	TP_PROTO(const char *name, unsigned int state, unsigned int cpu_id),

	TP_ARGS(name, state, cpu_id)
);

DEFINE_EVENT(clock, clock_disable,

	TP_PROTO(const char *name, unsigned int state, unsigned int cpu_id),

	TP_ARGS(name, state, cpu_id)
);

DEFINE_EVENT(clock, clock_set_rate,

	TP_PROTO(const char *name, unsigned int state, unsigned int cpu_id),

	TP_ARGS(name, state, cpu_id)
);

TRACE_EVENT(clock_set_parent,

	TP_PROTO(const char *name, const char *parent_name),

	TP_ARGS(name, parent_name),

	TP_STRUCT__entry(
		__string(       name,           name            )
		__string(       parent_name,    parent_name     )
	),

	TP_fast_assign(
		__assign_str(name, name);
		__assign_str(parent_name, parent_name);
	),

	TP_printk("%s parent=%s", __get_str(name), __get_str(parent_name))
);

/*
 * The power domain events are used for power domains transitions
 */
DECLARE_EVENT_CLASS(power_domain,

	TP_PROTO(const char *name, unsigned int state, unsigned int cpu_id),

	TP_ARGS(name, state, cpu_id),

	TP_STRUCT__entry(
		__string(       name,           name            )
		__field(        u64,            state           )
		__field(        u64,            cpu_id          )
	),

	TP_fast_assign(
		__assign_str(name, name);
		__entry->state = state;
		__entry->cpu_id = cpu_id;
),

	TP_printk("%s state=%lu cpu_id=%lu", __get_str(name),
		(unsigned long)__entry->state, (unsigned long)__entry->cpu_id)
);

DEFINE_EVENT(power_domain, power_domain_target,

	TP_PROTO(const char *name, unsigned int state, unsigned int cpu_id),

	TP_ARGS(name, state, cpu_id)
);

DECLARE_EVENT_CLASS(kpm_module,

	TP_PROTO(unsigned int managed_cpus, unsigned int max_cpus),

	TP_ARGS(managed_cpus, max_cpus),

	TP_STRUCT__entry(
		__field(u32, managed_cpus)
		__field(u32, max_cpus)
	),

	TP_fast_assign(
		__entry->managed_cpus = managed_cpus;
		__entry->max_cpus = max_cpus;
	),

	TP_printk("managed:%x max_cpus=%u", (unsigned int)__entry->managed_cpus,
					(unsigned int)__entry->max_cpus)
);

DEFINE_EVENT(kpm_module, set_max_cpus,
	TP_PROTO(unsigned int managed_cpus, unsigned int max_cpus),
	TP_ARGS(managed_cpus, max_cpus)
);

DEFINE_EVENT(kpm_module, reevaluate_hotplug,
	TP_PROTO(unsigned int managed_cpus, unsigned int max_cpus),
	TP_ARGS(managed_cpus, max_cpus)
);

TRACE_EVENT(core_ctl_eval_need,

	TP_PROTO(unsigned int cpu, unsigned int old_need,
		 unsigned int new_need, unsigned int updated),
	TP_ARGS(cpu, old_need, new_need, updated),
	TP_STRUCT__entry(
		__field(u32, cpu)
		__field(u32, old_need)
		__field(u32, new_need)
		__field(u32, updated)
	),
	TP_fast_assign(
		__entry->cpu = cpu;
		__entry->old_need = old_need;
		__entry->new_need = new_need;
		__entry->updated = updated;
	),
	TP_printk("cpu=%u, old_need=%u, new_need=%u, updated=%u", __entry->cpu,
		  __entry->old_need, __entry->new_need, __entry->updated)
);

TRACE_EVENT(core_ctl_set_busy,

	TP_PROTO(unsigned int cpu, unsigned int busy,
		 unsigned int old_is_busy, unsigned int is_busy),
	TP_ARGS(cpu, busy, old_is_busy, is_busy),
	TP_STRUCT__entry(
		__field(u32, cpu)
		__field(u32, busy)
		__field(u32, old_is_busy)
		__field(u32, is_busy)
	),
	TP_fast_assign(
		__entry->cpu = cpu;
		__entry->busy = busy;
		__entry->old_is_busy = old_is_busy;
		__entry->is_busy = is_busy;
	),
	TP_printk("cpu=%u, busy=%u, old_is_busy=%u, new_is_busy=%u",
		  __entry->cpu, __entry->busy, __entry->old_is_busy,
		  __entry->is_busy)
);

/*
 * The pm qos events are used for pm qos update
 */
DECLARE_EVENT_CLASS(pm_qos_update,

	TP_PROTO(enum pm_qos_req_action action, int prev_value, int curr_value),

	TP_ARGS(action, prev_value, curr_value),

	TP_STRUCT__entry(
		__field( enum pm_qos_req_action, action         )
		__field( int,                    prev_value     )
		__field( int,                    curr_value     )
	),

	TP_fast_assign(
		__entry->action = action;
		__entry->prev_value = prev_value;
		__entry->curr_value = curr_value;
	),

	TP_printk("action=%s prev_value=%d curr_value=%d",
		  __print_symbolic(__entry->action,
			{ PM_QOS_ADD_REQ,	"ADD_REQ" },
			{ PM_QOS_UPDATE_REQ,	"UPDATE_REQ" },
			{ PM_QOS_REMOVE_REQ,	"REMOVE_REQ" }),
		  __entry->prev_value, __entry->curr_value)
);

DEFINE_EVENT(pm_qos_update, pm_qos_update_target,

	TP_PROTO(enum pm_qos_req_action action, int prev_value, int curr_value),

	TP_ARGS(action, prev_value, curr_value)
);

DEFINE_EVENT_PRINT(pm_qos_update, pm_qos_update_flags,

	TP_PROTO(enum pm_qos_req_action action, int prev_value, int curr_value),

	TP_ARGS(action, prev_value, curr_value),

	TP_printk("action=%s prev_value=0x%x curr_value=0x%x",
		  __print_symbolic(__entry->action,
			{ PM_QOS_ADD_REQ,	"ADD_REQ" },
			{ PM_QOS_UPDATE_REQ,	"UPDATE_REQ" },
			{ PM_QOS_REMOVE_REQ,	"REMOVE_REQ" }),
		  __entry->prev_value, __entry->curr_value)
);
#endif /* _TRACE_POWER_H */

/* This part must be outside protection */
#include <trace/define_trace.h>
