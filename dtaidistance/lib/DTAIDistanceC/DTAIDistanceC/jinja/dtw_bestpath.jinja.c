/*!
{%- if "affinity" in suffix %}
Compute best path in affinity matrix for two series.
{%- else %}
Compute best path between two series.
{%- endif %}
 
 @param wps Array of length `(l1+1)*min(l2+1, abs(l1-l2) + 2*window-1)` with the warping paths.
 @param i1 Array of length l1+l2 to store the indices for the first sequence.
    Reverse ordered, last one is if i1 or i2 is zero.
 @param i2 Array of length l1+l2 to store the indices for the second sequence.
    Reverse ordered, last one is if i1 or i2 is zero.
 @param l1 Length of first array.
 @param l2 Length of second array.
 {%- if "affinity" in suffix %}
 @param rs Start position row.
 @param cs Start position column.
 {%- endif %}
 @param settings for Dynamic Time Warping.
 @return length of path
 */
{%- if "affinity" in suffix %}
{%- set cmp=">=" %}
{%- else %}
{%- set cmp="<=" %}
{%- endif %}
idx_t dtw_best_path{{suffix}}(seq_t *wps, idx_t *i1, idx_t *i2, idx_t l1, idx_t l2,
                    {% if "affinity" in suffix -%}
                    idx_t rs, idx_t cs,
                    {%- endif %}
                    DTWSettings *settings) {
    DTWWps p = dtw_wps_parts(l1, l2, settings);

    idx_t i = 0;
    {%- if "affinity" in suffix %}
    idx_t rip = rs;
    idx_t cip = cs;
    idx_t wpsi;
    {%- else %}
    idx_t rip = l1;
    idx_t cip = l2;
    idx_t min_ci;
    idx_t wpsi_start, wpsi;
    {%- endif %}
    idx_t ri_widthp = p.width * (rip - 1);
    idx_t ri_width = p.width * rip;

    // D. ri3 <= ri < l1
    {%- if "affinity" in suffix %}
    wpsi = dtw_wps_loc(&p, rs, cs, l1, l2) - ri_width;
    {%- else %}
    min_ci = p.ri3 + 1 - p.window - p.ldiff;
    wpsi_start = 2;
    if (p.ri2 == p.ri3) {
        wpsi_start = min_ci + 1;
    }
    wpsi = wpsi_start + (l2 - min_ci) - 1;
    {%- endif %}
    while (rip > p.ri3 && cip > 0) {
        {%- if "affinity" in suffix %}
        if (wps[ri_width + wpsi] <= 0) {
            return i;
        } else {
            i1[i] = rip - 1;
            i2[i] = cip - 1;
            // printf("wps[%zu,%zu] = wps[%zu] = %.3f\n", rip, cip, ri_width + wpsi, wps[ri_width + wpsi]);
            i++;
        }
        {%- else %}
        if (wps[ri_width + wpsi] != -1) {
            i1[i] = rip - 1;
            i2[i] = cip - 1;
            i++;
        }
        {%- endif %}
        if (wps[ri_widthp + wpsi - 1] {{cmp}} wps[ri_width  + wpsi - 1] &&
            wps[ri_widthp + wpsi - 1] {{cmp}} wps[ri_widthp + wpsi]) {
            // Go diagonal
            cip--;
            rip--;
            wpsi--;
            ri_width = ri_widthp;
            ri_widthp -= p.width;
        } else if (wps[ri_width + wpsi - 1] {{cmp}} wps[ri_widthp + wpsi]) {
            // Go left
            cip--;
            wpsi--;
        } else {
            // Go up
            rip--;
            ri_width = ri_widthp;
            ri_widthp -= p.width;
        }
    }

    // C. ri2 <= ri < ri3
    while (rip > p.ri2 && cip > 0) {
        {%- if "affinity" in suffix %}
        if (wps[ri_width + wpsi] <= 0) {
            return i;
        } else {
            i1[i] = rip - 1;
            i2[i] = cip - 1;
            // printf("wps[%zu,%zu] = wps[%zu] = %.3f\n", rip, cip, ri_width + wpsi, wps[ri_width + wpsi]);
            i++;
        }
        {%- else %}
        if (wps[ri_width + wpsi] != -1) {
            i1[i] = rip - 1;
            i2[i] = cip - 1;
            i++;
        }
        {%- endif %}
        if (wps[ri_widthp + wpsi] {{cmp}} wps[ri_width  + wpsi - 1] &&
            wps[ri_widthp + wpsi] {{cmp}} wps[ri_widthp + wpsi + 1]) {
            // Go diagonal
            cip--;
            rip--;
            ri_width = ri_widthp;
            ri_widthp -= p.width;
        } else if (wps[ri_width + wpsi - 1] {{cmp}} wps[ri_widthp + wpsi + 1]) {
            // Go left
            cip--;
            wpsi--;
        } else {
            // Go up
            rip--;
            wpsi++;
            ri_width = ri_widthp;
            ri_widthp -= p.width;
        }
    }

    // A-B. 0 <= ri < ri2
    while (rip > 0 && cip > 0) {
        {%- if "affinity" in suffix %}
        if (wps[ri_width + wpsi] <= 0) {
            return i;
        } else {
            i1[i] = rip - 1;
            i2[i] = cip - 1;
            // printf("wps[%zu,%zu] = wps[%zu] = %.3f\n", rip, cip, ri_width + wpsi, wps[ri_width + wpsi]);
            i++;
        }
        {%- else %}
        if (wps[ri_width + wpsi] != -1) {
            i1[i] = rip - 1;
            i2[i] = cip - 1;
            i++;
        }
        {%- endif %}
        if (wps[ri_widthp + wpsi - 1] {{cmp}} wps[ri_width  + wpsi - 1] &&
            wps[ri_widthp + wpsi - 1] {{cmp}} wps[ri_widthp + wpsi]) {
            // Go diagonal
            cip--;
            rip--;
            wpsi--;
            ri_width = ri_widthp;
            ri_widthp -= p.width;
        } else {
            if (wps[ri_width + wpsi - 1] {{cmp}} wps[ri_widthp + wpsi]) {
                // Go left
                cip--;
                wpsi--;
            } else {
                // Go up
                rip--;
                ri_width = ri_widthp;
                ri_widthp -= p.width;
            }
        }
    }
    return i;
}
