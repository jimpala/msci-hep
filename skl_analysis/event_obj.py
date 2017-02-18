

class Event:

    # Static variables

    __process_general_map = {
        'qqZvvH125': 'VH',
        'qqWlvH125': 'VH',
        'Wbb': 'V+jets',
        'Wbc': 'V+jets',
        'Wcc': 'V+jets',
        'Wbl': 'V+jets',
        'Wcl': 'V+jets',
        'Wl': 'V+jets',
        'Zbb': 'V+jets',
        'Zbc': 'V+jets',
        'Zcc': 'V+jets',
        'Zbl': 'V+jets',
        'Zcl': 'V+jets',
        'Zl': 'V+jets',
        'ttbar': 'ttbar',
        'stopt': 'stop',
        'stops': 'stop',
        'stopWt': 'stop',
        'WW': 'diboson',
        'ZZ': 'diboson',
        'WZ': 'diboson'
    }

    __scale_factor_map = {
        2: {
            'Zl': 1.0,
            'Zcl': 1.41,
            'Zcc': 1.15,
            'Zbl': 1.15,
            'Zbc': 1.15,
            'Zbb': 1.15,
            'Wl': 1.00,
            'Wcl': 1.22,
            'Wcc': 1.65,
            'Wbl': 1.65,
            'Wbc': 1.65,
            'Wbb': 1.65,
            'stopWt': 0.92,
            'stopt': 0.92,
            'stops': 0.92,
            'ttbar': 0.80,
            'multijet': 0.63,
            'WW': 1.13,
            'ZZ': 1.13,
            'WZ': 1.13,
            'qqZvvH125': 0.20,
            'qqWlvH125': 1.0
        },

        3: {

            'Zl': 1.0,
            'Zcl': 1.0,
            'Zcc': 1.1,
            'Zbl': 1.1,
            'Zbc': 1.1,
            'Zbb': 1.1,
            'Wl': 1.00,
            'Wcl': 1.27,
            'Wcc': 1.59,
            'Wbl': 1.59,
            'Wbc': 1.59,
            'Wbb': 1.59,
            'stopWt': 0.99,
            'stopt': 0.99,
            'stops': 0.99,
            'ttbar': 0.87,
            'multijet': 0.6,
            'WW': 1.12,
            'ZZ': 1.12,
            'WZ': 1.12,
            'qqZvvH125': 0.21,
            'qqWlvH125': 0.21
        }
    }



    # Constructor

    def __init__(self, process, n_jets, event_no, event_weight):
        self.process = process
        self.process_general = Event.__process_general_map[process]
        self.classification = 1 if self.process_general == 'VH' else 0
        self.n_jets = n_jets
        self.event_no = event_no
        self.event_weight = event_weight
        self.scale_factor = Event.__scale_factor_map[self.n_jets][self.process]

        self.split = None
        self.decision_value = None

    def splitting(self, index):
        self.split = index

    def is_in_bin(self, low_edge, high_edge):
        if self.decision_value >= low_edge and self.decision_value < high_edge:
            return 1
        else:
            return 0



