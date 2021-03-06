SIMULATION = {'simulation': {'archetypes': {'spec': [
                                        {'lib': 'cyclus.pyagents', 'name': 'Sink'},
                                        {'lib': 'cyclus.pyagents', 'name': 'NullRegion'},
                                        {'lib': 'cyclus.pyagents', 'name': 'NullInst'}
                                        ]},
                'control': {'duration': '100',
                            'solver': {'config': {'coin-or': {'timeout': '10'}}},
                            'startmonth': '1',
                            'startyear': '2000'},
                'facility': {'config': {'Sink': {'capacity': '1.00',
                                                 'in_commods': {'val': 'commodity'}}},
                             'name': 'Sink'},
                'recipe': {'basis': 'mass',
                           'name': 'commod_recipe',
                           'nuclide': {'comp': '1', 'id': '010010000'}},
                'region': {'config': {'NullRegion': None},
                           'institution': {'config': {'NullInst': None},
                                           'initialfacilitylist': {'entry': {'number': '1',
                                                                             'prototype': 'Sink'}},
                                           'name': 'SingleInstitution'},
                           'name': 'SingleRegion'}}}
